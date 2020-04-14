#include <QDebug>
#include <QtMath>
#include <windows.h>
#include "controller.h"

typedef DWORD (WINAPI *XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE *pState);
static XInputGetState_t XInputGetStateL;

Controller::Controller() : m_connected(false),
    m_connectCheckTimer(), m_state()
{
}

bool Controller::readCurrentState(quint32 index)
{
    assert(index < XUSER_MAX_COUNT);

    if (!m_connected && m_connectCheckTimer.isValid() &&
            m_connectCheckTimer.elapsed() < 3000)
    {
        return false;
    }

    DWORD dwResult = XInputGetStateL(index, &m_state[Controller::CURRENT_STATE]);
    m_connected = (dwResult == ERROR_SUCCESS);

    if (!m_connected)
        m_connectCheckTimer.restart();

    return m_connected;
}

bool Controller::buttonIsDown(DWORD button, quint32 state) const
{
    return m_connected && (m_state[state].Gamepad.wButtons & button) != 0;
}

ButtonState Controller::getButtonState(DWORD button) const
{
    if (buttonIsDown(button, PREVIOUS_STATE))
    {
        if (buttonIsDown(button, CURRENT_STATE))
            return ButtonState::StillDown;
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

ControllerThread::ControllerThread() : m_controller(),
    m_leftThumbDeadZone(DEF_JOYSTICK_DEADZONE), m_shouldStop(false), m_enabled(true)
{
}

ControllerThread& ControllerThread::instance()
{
    static ControllerThread c;
    return c;
}

qint32 ControllerThread::getNormDeadZone(SHORT value, SHORT deadZone)
{
    if (abs(value) < deadZone)
        return 0;
    else
        return ((value < 0) ? -1 : 1) * (abs(value) - deadZone);
}

void ControllerThread::updateMousePosition(const Controller &controller, double delta)
{
    if (!m_enabled)
        return;

    const XINPUT_GAMEPAD &gamepad = controller.m_state[Controller::CURRENT_STATE].Gamepad;
    int tlx = getNormDeadZone(gamepad.sThumbLX, m_leftThumbDeadZone);
    int tly = getNormDeadZone(gamepad.sThumbLY, m_leftThumbDeadZone);
    if (tlx != 0 || tly != 0)
    {
        POINT p;
        if (GetCursorPos(&p))
        {
            double step = (SPEED / FPS) * delta;
            double x = (tlx / (double)(JOYSTICK_MAX_VALUE - m_leftThumbDeadZone)) * step;
            double y = (tly / (double)(JOYSTICK_MAX_VALUE - m_leftThumbDeadZone)) * step;

            p.x += (int)x;
            p.y -= (int)y;

            SetCursorPos(p.x, p.y);
        }
    }
}

void ControllerThread::triggerMouseEvent(const Controller& controller)
{
    if (!m_enabled)
        return;

    DWORD dwFlags = 0;
    ButtonState buttonState = controller.getButtonState(XINPUT_GAMEPAD_A);
    if (buttonState == ButtonState::Down)
        dwFlags |= MOUSEEVENTF_LEFTDOWN;
    else if (buttonState == ButtonState::Up)
        dwFlags |= MOUSEEVENTF_LEFTUP;

    buttonState = controller.getButtonState(XINPUT_GAMEPAD_X);
    if (buttonState == ButtonState::Down)
        dwFlags |= MOUSEEVENTF_RIGHTDOWN;
    else if (buttonState == ButtonState::Up)
        dwFlags |= MOUSEEVENTF_RIGHTUP;

    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = dwFlags;
    SendInput(1, &input, sizeof(input));
}

void ControllerThread::run()
{
    quint32 elapsed = 0;
    QElapsedTimer fpsTimer;

    qInfo() << "Controller thread started";

    while (!m_shouldStop)
    {
        fpsTimer.restart();
        for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
        {
            Controller& controller = m_controller[i];

            bool connected = controller.isConnected();
            controller.readCurrentState(i);

            if (!connected && controller.isConnected())
                qInfo().nospace() << "Controller #" << i << " is connected";
            else if (connected && !controller.isConnected())
                qInfo().nospace() << "Controller #" << i << " is disconnected";

            triggerMouseEvent(controller);
            updateMousePosition(controller, elapsed / FRAME_DURATION);

            ButtonState start = controller.getButtonState(XINPUT_GAMEPAD_START);
            ButtonState back = controller.getButtonState(XINPUT_GAMEPAD_BACK);
            if (start == ButtonState::Up && !controller.m_startButtonHistory.IsStillActive(start))
                controller.m_startButtonHistory.StartActive(ButtonState::Up);
            if (back == ButtonState::Up && !controller.m_backButtonHistory.IsStillActive(back))
                controller.m_backButtonHistory.StartActive(ButtonState::Up);
            if (controller.m_startButtonHistory.IsStillActive(ButtonState::Up) &&
                    controller.m_backButtonHistory.IsStillActive(ButtonState::Up))
            {
                controller.m_startButtonHistory.Clear();
                controller.m_backButtonHistory.Clear();
                setEnabled(!m_enabled);
                if (m_enabled)
                    qInfo().nospace() << "Controller thread enabled";
                else
                    qInfo().nospace() << "Controller thread disabled";
            }

            controller.saveCurrentState();
        }

        elapsed = fpsTimer.elapsed();
        qint32 sleep_time = FRAME_DURATION - elapsed;
        if (sleep_time > 10)
        {
            fpsTimer.restart();
            msleep(sleep_time);
            elapsed += fpsTimer.elapsed();
        }
    }

    qInfo() << "Controller thread stopped";
}

bool ControllerThread::start()
{
    m_lib.setFileName(QStringLiteral("xinput1_4.dll"));
    if (!m_lib.load())
    {
        m_lib.setFileName(QStringLiteral("xinput1_3.dll"));
        m_lib.load();
    }

    if (!m_lib.isLoaded())
        return false;

    XInputGetStateL = (XInputGetState_t) m_lib.resolve("XInputGetState");
    if (XInputGetStateL == NULL)
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
