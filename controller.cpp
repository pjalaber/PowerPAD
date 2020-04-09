#include <QCursor>
#include <QApplication>
#include <QException>
#include <QDebug>
#include <windows.h>
#include "controller.h"

typedef DWORD (WINAPI *XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE *pState);
static XInputGetState_t XInputGetStateL;

typedef DWORD (WINAPI *XInputGetCapabilities_t)(DWORD, DWORD, XINPUT_CAPABILITIES*);
static XInputGetCapabilities_t XInputGetCapabilitiesL;

Controller::Controller() : m_connected(false), m_controlMouse(true),
m_state(), m_lastStateCheckElapsedTimer()
{
}

bool Controller::buttonIsDown(DWORD button, UINT state) const
{
	return m_connected && (m_state[state].Gamepad.wButtons & button) != 0;
}

ButtonState Controller::getButtonState(DWORD button) const
{
    if (buttonIsDown(button, PREVIOUS_STATE))
	{
        if (buttonIsDown(button, CURRENT_STATE))
			return ButtonState::Down;
		else
			return ButtonState::Up;
	}
	else
	{
        if (buttonIsDown(button, CURRENT_STATE))
			return ButtonState::Down;
		else
			return ButtonState::None;
	}
}

void Controller::saveCurrentState(void)
{
	m_state[PREVIOUS_STATE] = m_state[CURRENT_STATE];
}

ControllerThread::ControllerThread() : m_leftThumbDeadZone(DEF_JOYSTICK_DEADZONE),
    m_horiz_speed(DEF_HORIZ_SPEED), m_vert_speed(DEF_VERT_SPEED), m_shouldStop(false)
{
}

INT ControllerThread::getNormDeadZone(SHORT value, SHORT deadZone)
{
	if (abs(value) < deadZone)
		return 0;
	else
		return ((value < 0) ? -1 : 1) * (abs(value) - deadZone);
}

void ControllerThread::updateMousePosition(const Controller &controller)
{
	if (!controller.m_controlMouse)
		return;
	
	const XINPUT_GAMEPAD &gamepad = controller.m_state[Controller::CURRENT_STATE].Gamepad;
    INT tlx = getNormDeadZone(gamepad.sThumbLX, m_leftThumbDeadZone);
    INT tly = getNormDeadZone(gamepad.sThumbLY, m_leftThumbDeadZone);
	if (tlx != 0 || tly != 0)
	{
        QPoint p = QCursor::pos();
        int x = p.x() + (int)(tlx / ((double)JOYSTICK_MAX_VALUE - m_leftThumbDeadZone) * m_horiz_speed);
        int y = p.y() - (int)(tly / ((double)JOYSTICK_MAX_VALUE - m_leftThumbDeadZone) * m_vert_speed);
        QCursor::setPos(x, y);
	}
}

void ControllerThread::triggerMouseEvent(const Controller& controller)
{
	if (!controller.m_controlMouse)
		return;

	DWORD evFlags = 0;
    ButtonState buttonState = controller.getButtonState(XINPUT_GAMEPAD_A);
	if (buttonState == ButtonState::Down)
		evFlags |= MOUSEEVENTF_LEFTDOWN;
	else if (buttonState == ButtonState::Up)
		evFlags |= MOUSEEVENTF_LEFTUP;
	
    buttonState = controller.getButtonState(XINPUT_GAMEPAD_X);
	if (buttonState == ButtonState::Down)
		evFlags |= MOUSEEVENTF_RIGHTDOWN;
	else if (buttonState == ButtonState::Up)
		evFlags |= MOUSEEVENTF_RIGHTUP;
	
	if (evFlags != 0)
		mouse_event(evFlags, 0, 0, 0, 0);
}

void ControllerThread::run()
{
    qInfo() << "Controller thread started";
	while (!m_shouldStop)
	{
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			Controller& controller = m_controller[i];
            if (!controller.m_connected && (
                        !controller.m_lastStateCheckElapsedTimer.isValid() ||
                        controller.m_lastStateCheckElapsedTimer.elapsed() >= 3000)
                    )
			{
				XINPUT_CAPABILITIES cap;
                DWORD dwResult = XInputGetCapabilitiesL(i, XINPUT_FLAG_GAMEPAD, &cap);
				if (dwResult == ERROR_SUCCESS)
				{
                    qInfo() << "Controller #" << i << " is connected";
					controller.m_connected = true;
				}
                controller.m_lastStateCheckElapsedTimer.start();
			}
		}
		
		for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
		{
			Controller& controller = m_controller[i];
			if (!controller.m_connected)
				continue;

            DWORD dwResult = XInputGetStateL(i, &controller.m_state[Controller::CURRENT_STATE]);
			if (dwResult != ERROR_SUCCESS)
			{
                qInfo() << "Controller #" << i << " is disconnected";
				controller.m_connected = false;
				continue;
			}
			
            updateMousePosition(controller);
            triggerMouseEvent(controller);

            ButtonState buttonState1 = controller.getButtonState(XINPUT_GAMEPAD_START);
            ButtonState buttonState2 = controller.getButtonState(XINPUT_GAMEPAD_BACK);
			if (buttonState1 == ButtonState::Up && !controller.m_startButtonHistory.IsStillActive(buttonState1))
				controller.m_startButtonHistory.StartActive(ButtonState::Up);
			if (buttonState2 == ButtonState::Up && !controller.m_backButtonHistory.IsStillActive(buttonState2))
				controller.m_backButtonHistory.StartActive(ButtonState::Up);
			if (controller.m_startButtonHistory.IsStillActive(ButtonState::Up) && controller.m_backButtonHistory.IsStillActive(ButtonState::Up))
			{
				controller.m_startButtonHistory.Clear();
				controller.m_backButtonHistory.Clear();
				controller.m_controlMouse = !controller.m_controlMouse;
				if (controller.m_controlMouse)
                    qInfo() << "Controller #" << i << " now controls mouse";
				else
                    qInfo() << "Controller #" << i << " doesn't control mouse any more";
			}
			
            controller.saveCurrentState();
		}
        msleep(1000 / MOUSE_FPS);
	}
     qInfo() << "Controller thread stopped";
}

bool ControllerThread::start()
{
    m_lib.setFileName(QStringLiteral("xinput1_4.dll"));
    if (!m_lib.load()) {
        m_lib.setFileName(QStringLiteral("xinput1_3.dll"));
        m_lib.load();
    }

    if (!m_lib.isLoaded())
         return false;

    XInputGetStateL = (XInputGetState_t) m_lib.resolve("XInputGetState");
    if (XInputGetStateL == NULL)
        return false;

    XInputGetCapabilitiesL = (XInputGetCapabilities_t) m_lib.resolve("XInputGetCapabilities");
    if (XInputGetCapabilitiesL == NULL)
        return false;

    m_shouldStop = false;
    QThread::start();
    return true;
}

void ControllerThread::stop()
{
	m_shouldStop = true;
    wait();
}
