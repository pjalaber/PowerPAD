#include <QDebug>
#include <QtMath>
#include <windows.h>
#include "controller.h"

typedef DWORD (WINAPI *XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE *pState);
static XInputGetState_t XInputGetStateL;

Controller::Controller() : m_connected(false),
    m_connectCheckTimer(), m_state(), m_accelerationGraceTimeState(false),  m_accelerationTimer()
{
}

bool Controller::readCurrentState(quint32 index)
{
    Q_ASSERT(index < XUSER_MAX_COUNT);

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
    m_shouldStop(false), m_enabled(true),
    m_connectedCount(0), m_status(StatusOK), m_settings(Settings::instance())
{
}

ControllerThread* ControllerThread::instance()
{
    static ControllerThread *c = new ControllerThread();
    return c;
}

qint32 ControllerThread::getNormDeadZone(SHORT value, SHORT deadZone)
{
    if (abs(value) < deadZone)
        return 0;
    else
        return ((value < 0) ? -1 : 1) * (abs(value) - deadZone);
}

void ControllerThread::updateMousePosition(Controller &controller, double delta)
{
    if (!m_enabled)
        return;

    quint32 leftDeadZone = m_settings->leftJoystickDeadZone();
    const XINPUT_GAMEPAD &gamepad = controller.m_state[Controller::CURRENT_STATE].Gamepad;

    int tlx = getNormDeadZone(gamepad.sThumbLX, leftDeadZone);
    int tly = getNormDeadZone(gamepad.sThumbLY, leftDeadZone);

    POINT p;
    if ((tlx != 0 || tly != 0) && GetCursorPos(&p))
    {
        qint32 maxThumb = INT16_MAX - leftDeadZone;
        bool accHint = (abs(tlx) >= maxThumb || abs(tly) >= maxThumb);
        controller.m_mouseAcceleration.setAccelerationHint(accHint);
        double acc = controller.m_mouseAcceleration.isAccelerationOn() ? m_settings->mouseAcceleration() : 1.0;

        double step = (SPEED * m_settings->mouseSpeed() * acc / FPS) * delta;
        double x = (tlx / (double)maxThumb) * step;
        double y = (tly / (double)maxThumb) * step;

        p.x += (int)x;
        p.y -= (int)y;

        SetCursorPos(p.x, p.y);
    }
}

void ControllerThread::triggerMouseWheel(Controller &controller)
{
    if (!m_enabled)
        return;

    quint32 rightDeadZone = m_settings->rightJoystickDeadZone();
    const XINPUT_GAMEPAD &gamepad = controller.m_state[Controller::CURRENT_STATE].Gamepad;

    int tly = getNormDeadZone(gamepad.sThumbRY, rightDeadZone);
    if ( tly != 0) {
        quint32 scroll = (tly / (double)(INT16_MAX - rightDeadZone)) * m_settings->mouseScrollSpeed() * 30;

        INPUT input = {};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        input.mi.mouseData = scroll;
        SendInput(1, &input, sizeof(input));
    }
}

void ControllerThread::triggerMouseButton(const Controller& controller)
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
            {
                setConnectedCount(m_connectedCount + 1);
                qInfo().nospace() << "Controller #" << i << " is connected";
            }
            else if (connected && !controller.isConnected())
            {
                setConnectedCount(m_connectedCount - 1);
                qInfo().nospace() << "Controller #" << i << " is disconnected";
            }

            triggerMouseButton(controller);
            triggerMouseWheel(controller);
            updateMousePosition(controller, elapsed / FRAME_DURATION);

            ButtonState back = controller.getButtonState(XINPUT_GAMEPAD_BACK);
            ButtonState start = controller.getButtonState(XINPUT_GAMEPAD_START);
            if (start == ButtonState::Up && !controller.m_startButtonStateTimer.isStillActive(start))
                controller.m_startButtonStateTimer.startActive(ButtonState::Up);
            if (back == ButtonState::Up && !controller.m_backButtonStateTimer.isStillActive(back))
                controller.m_backButtonStateTimer.startActive(ButtonState::Up);
            if (controller.m_startButtonStateTimer.isStillActive(ButtonState::Up) &&
                    controller.m_backButtonStateTimer.isStillActive(ButtonState::Up))
            {
                controller.m_startButtonStateTimer.clear();
                controller.m_backButtonStateTimer.clear();
                setEnabled(!m_enabled);
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
    if (!m_lib.load()) {
        m_lib.setFileName(QStringLiteral("xinput1_3.dll"));
        m_lib.load();
    }

    if (!m_lib.isLoaded()) {
        setStatus(StatusXInputLibraryNotFound);
        return false;
    }

    XInputGetStateL = (XInputGetState_t) m_lib.resolve("XInputGetState");
    if (XInputGetStateL == NULL) {
        setStatus(StatusXInputSymbolNotFound);
        return false;
    }


    m_shouldStop = false;
    QThread::start();
    return true;
}

void ControllerThread::stop()
{
    m_shouldStop = true;
    wait();
}

bool ControllerThread::enabled()
{
    return m_enabled;
}

void ControllerThread::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        emit enabledChanged();
        if (m_enabled)
            qInfo().nospace() << "Controller thread enabled";
        else
            qInfo().nospace() << "Controller thread disabled";
    }
}


quint32 ControllerThread::connectedCount()
{
    return m_connectedCount;
}

void ControllerThread::setConnectedCount(quint32 connectedCount)
{
    Q_ASSERT(connectedCount <= XUSER_MAX_COUNT);
    if (m_connectedCount != connectedCount) {
        m_connectedCount = connectedCount;
        emit connectedCountChanged();
    }
}

ControllerThread::Status ControllerThread::status()
{
    return m_status;
}

void ControllerThread::setStatus(Status status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}
