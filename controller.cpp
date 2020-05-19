#include <QDebug>
#include <QtMath>
#include <QApplication>
#include "controller.h"

Controller::Controller() : m_connected(false),
    m_connectCheckTimer(), m_state(), m_accelerationGraceTimeState(false),  m_accelerationTimer(),
    m_enableDisableButtonCombo(ButtonState::Up)
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

    quint32 result = WinSys::XInputGetState(index, &m_state[Controller::CURRENT_STATE]);
    m_connected = (result == 0);

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
    m_connectedCount(0), m_action(Action::instance()), m_winsys(WinSys::instance()),
    m_settings(Settings::instance()), m_keyboard(Keyboard::instance())
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
    if (!m_enabled || m_keyboard->show())
        return;

    const XINPUT_GAMEPAD &gamepad = controller.m_state[Controller::CURRENT_STATE].Gamepad;

    qint32 tlx, tly;
    quint32 deadZone;

    if (m_action->find(Action::ControllerAnalogAction::MouseMove) == Action::ControllerAnalog::Left) {
        deadZone = m_settings->leftJoystickDeadZone();
        tlx = getNormDeadZone(gamepad.sThumbLX, deadZone);
        tly = getNormDeadZone(gamepad.sThumbLY, deadZone);
    }
    else {
        deadZone = m_settings->rightJoystickDeadZone();
        tlx = getNormDeadZone(gamepad.sThumbRX, deadZone);
        tly = getNormDeadZone(gamepad.sThumbRY, deadZone);
    }

    QPoint p;
    if ((tlx != 0 || tly != 0) && WinSys::getMouseCursorPos(p))
    {
        qint32 maxThumb = INT16_MAX - deadZone;
        bool accHint = (abs(tlx) >= 0.97 * maxThumb || abs(tly) >= 0.97 * maxThumb);
        controller.m_mouseAcceleration.setAccelerationHint(accHint);
        double acc = controller.m_mouseAcceleration.isAccelerationOn() ? m_settings->mouseAcceleration() : 1.0;

        double step = (SPEED * m_settings->mouseSpeed() * acc / FPS) * delta;
        double x = (tlx / (double)maxThumb) * step;
        double y = (tly / (double)maxThumb) * step;

        p.rx() += (qint32)x;
        p.ry() -= (qint32)y;

        WinSys::setMouseCursorPos(p);
    }
}

void ControllerThread::triggerMouseWheel(Controller &controller)
{
    if (!m_enabled || m_keyboard->show())
        return;

    const XINPUT_GAMEPAD &gamepad = controller.m_state[Controller::CURRENT_STATE].Gamepad;

    qint32 tly;
    quint32 deadZone;

    if (m_action->find(Action::ControllerAnalogAction::MouseWheel) == Action::ControllerAnalog::Left) {
        deadZone = m_settings->leftJoystickDeadZone();
        tly = getNormDeadZone(gamepad.sThumbLY, deadZone);
    }
    else {
        deadZone = m_settings->rightJoystickDeadZone();
        tly = getNormDeadZone(gamepad.sThumbRY, deadZone);
    }

    if (tly != 0) {
        qint32 scroll = (tly / (double)(INT16_MAX - deadZone)) * m_settings->mouseScrollSpeed() * 30;
        WinSys::sendMouseWheel(scroll);
    }
}

void ControllerThread::triggerMouseButton(const Controller& controller)
{
    if (!m_enabled || m_keyboard->show())
        return;

    WinSys::MouseButton leftButton;
    quint32 button = Action::getXInputButton(m_action->find(Action::ControllerButtonAction::MouseLeftClick));
    ButtonState buttonState = controller.getButtonState(button);
    if (buttonState == ButtonState::Down)
        leftButton = WinSys::MouseButton::Down;
    else if (buttonState == ButtonState::Up)
        leftButton = WinSys::MouseButton::Up;
    else
        leftButton = WinSys::MouseButton::None;

    WinSys::MouseButton rightButton;
    button = Action::getXInputButton(m_action->find(Action::ControllerButtonAction::MouseRightClick));
    buttonState = controller.getButtonState(button);
    if (buttonState == ButtonState::Down)
        rightButton = WinSys::MouseButton::Down;
    else if (buttonState == ButtonState::Up)
        rightButton = WinSys::MouseButton::Up;
    else
        rightButton = WinSys::MouseButton::None;

    WinSys::sendMouseButton(leftButton, rightButton);
}

void ControllerThread::handleAction(Controller& controller, Action::ControllerButtonAction action)
{
    Action::ControllerButton button = m_action->find(action);
    if (button == Action::ControllerButton::None)
        return;

    quint16 key = 0;
    bool isVirtual = false;

    switch (action) {
    case Action::ControllerButtonAction::KeyboardEnterKey:
        key = m_keyboard->getCharacterAt(m_keyboard->characterIndex()).unicode();
        isVirtual = false;
        break;
    case Action::ControllerButtonAction::Backspace:
        key = VK_BACK;
        isVirtual = true;
        break;
    case Action::ControllerButtonAction::Left:
        key = VK_LEFT;
        isVirtual = true;
        break;
    case Action::ControllerButtonAction::Right:
        key = VK_RIGHT;
        isVirtual = true;
        break;
    case Action::ControllerButtonAction::Up:
        key = VK_UP;
        isVirtual = true;
        break;
    case Action::ControllerButtonAction::Down:
        key = VK_DOWN;
        isVirtual = true;
        break;
    case Action::ControllerButtonAction::Enter:
        key = VK_RETURN;
        isVirtual = true;
        break;
    case Action::ControllerButtonAction::MediaPlayPause:
        key = VK_MEDIA_PLAY_PAUSE;
        isVirtual = true;
        break;
    default:
        Q_ASSERT(0);
    }

    ButtonState buttonState = controller.getButtonState(Action::getXInputButton(button));
    switch (buttonState) {
    case ButtonState::Down:
        WinSys::sendKeyDown(isVirtual, key);
        break;

    case ButtonState::StillDown:
        if (!controller.m_repeatTimer.isValid()) {
            controller.m_repeatTimer.start(buttonState, 400);
        }
        else if (controller.m_repeatTimer.hasExpired()) {
            controller.m_repeatTimer.start(buttonState, 25);
            WinSys::sendKeyDown(isVirtual, key);
        }
        break;

    case ButtonState::Up:
        controller.m_repeatTimer.invalidate();
        WinSys::sendKeyUp(isVirtual);
        break;

    default:
        break;
    }
}

void ControllerThread::handleKeyboard(Controller& controller, double delta)
{
    if (!m_enabled)
        return;

    quint32 button = Action::getXInputButton(m_action->find(Action::ControllerButtonAction::KeyboardActivate));
    ButtonState buttonState = controller.getButtonState(button);
    if (buttonState == ButtonState::Up)
        m_keyboard->setShow(!m_keyboard->show());

    if (!m_keyboard->show())
        return;

    const XINPUT_GAMEPAD &gamepad = controller.m_state[Controller::CURRENT_STATE].Gamepad;

    quint32 deadZone;
    qint32 tlx;
    if (m_action->find(Action::ControllerAnalogAction::KeyboardScroll) == Action::ControllerAnalog::Left) {
        deadZone = m_settings->leftJoystickDeadZone();
        tlx = getNormDeadZone(gamepad.sThumbLX, deadZone);
    }
    else {
        deadZone = m_settings->rightJoystickDeadZone();
        tlx = getNormDeadZone(gamepad.sThumbRX, deadZone);
    }

    if (tlx != 0) {
        qint32 maxThumb = INT16_MAX - deadZone;
        bool accHint = (abs(tlx) >= maxThumb);
        controller.m_mouseAcceleration.setAccelerationHint(accHint);
        double acc = controller.m_mouseAcceleration.isAccelerationOn() ? m_settings->mouseAcceleration() : 1.0;
        double step = (2 * m_settings->mouseSpeed() * acc / FPS) * delta;
        double x = (tlx / (double)maxThumb) * step;
        m_keyboard->incCharacterIndex(x);
    }

    for (Action::ControllerButtonAction action: {
         Action::ControllerButtonAction::KeyboardEnterKey,     
         })
    {
        handleAction(controller, action);
    }
}

void ControllerThread::handleComboButtons(Controller& controller)
{
    if (m_keyboard->show())
        return;

    /* enable/disable buttons combo
     */
    const QPair<Action::ControllerButton, Action::ControllerButton> *combo =
            m_action->find(Action::ControllerComboAction::EnableDisableApp);
    if (combo != nullptr) {
        ButtonState button1 = controller.getButtonState(Action::getXInputButton(combo->first));
        ButtonState button2 = controller.getButtonState(Action::getXInputButton(combo->second));
        controller.m_enableDisableButtonCombo.updateState(button1, button2, 200);
        if (controller.m_enableDisableButtonCombo.isComboOn())
        {
            controller.m_enableDisableButtonCombo.clear();
            setEnabled(!m_enabled);
        }
    }
}

void ControllerThread::handleButtons(Controller &controller)
{
    if (!m_enabled)
        return;

    /* left, right, up, down buttons
     */
    for (Action::ControllerButtonAction action: {
         Action::ControllerButtonAction::Left,
         Action::ControllerButtonAction::Right,
         Action::ControllerButtonAction::Up,
         Action::ControllerButtonAction::Down,
         Action::ControllerButtonAction::Backspace,
         Action::ControllerButtonAction::Enter
         })
    {
        handleAction(controller, action);
    }
}

void ControllerThread::run()
{
    quint32 elapsed = 0;
    QElapsedTimer fpsTimer, fullscreenCheckTimer;
    bool fullscreen = false;

    qInfo() << "Controller thread started";

    fullscreenCheckTimer.start();

    while (!m_shouldStop)
    {
        fpsTimer.restart();

        if (fullscreenCheckTimer.elapsed() >= 1000) {
            bool nowFullscreen = WinSys::isFullScreen();
            if (!fullscreen && nowFullscreen) {
                m_keyboard->setShow(false);
                setEnabled(false);
                fullscreen = true;
            }
            else if (fullscreen && !nowFullscreen) {
                setEnabled(true);
                fullscreen = false;
            }
            fullscreenCheckTimer.restart();
        }

        for (quint32 i = 0; i < XUSER_MAX_COUNT; i++)
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

            handleKeyboard(controller, elapsed / FRAME_DURATION);
            updateMousePosition(controller, elapsed / FRAME_DURATION);
            triggerMouseButton(controller);
            triggerMouseWheel(controller);
            handleButtons(controller);
            handleComboButtons(controller);            

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

void ControllerThread::start()
{
    if (m_winsys->status() == WinSys::StatusOK) {
        m_shouldStop = false;
        QThread::start();
    }
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
