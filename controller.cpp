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

double ControllerThread::square(double value)
{
    double res = value * value;

    if (value < 0)
        res = -res;

    return res;
}

void ControllerThread::updateMousePosition(Controller &controller, double delta)
{
    if (!m_enabled || m_keyboard->show())
        return;

    const XINPUT_GAMEPAD &gamepad = controller.m_state[Controller::CURRENT_STATE].Gamepad;

    double tlx, tly;
    quint32 deadZone;

    if (m_action->find(Action::ControllerAnalogAction::MouseMove) == Action::ControllerAnalog::Left) {
        deadZone = m_settings->leftJoystickDeadZone();
        tlx = gamepad.sThumbLX;
        tly = gamepad.sThumbLY;
    }
    else {
        deadZone = m_settings->rightJoystickDeadZone();
        tlx = gamepad.sThumbRX;
        tly = gamepad.sThumbRY;
    }

    // compute magnitude
    double mg = sqrt(tlx * tlx + tly * tly);

    QPoint p;
    if (mg > deadZone && WinSys::getMouseCursorPos(p)) {
        if (mg > 32767)
            mg = 32767;
        mg -= deadZone;

        double maxThumb = 32767 - deadZone;

        // normalized magnitude between [0.0, 1.0]
        double nmg = mg / maxThumb;

        controller.m_mouseAcceleration.setAccelerationHint(nmg);
        double acc = 1.0;
        if (controller.m_mouseAcceleration.isAccelerationOn())
            acc += m_settings->mouseAcceleration() / 10;

        double speed = SPEED * (m_settings->mouseSpeed() + 1.0);
        double step = (speed * acc / FPS) * delta;

        // normalized tlx and tly between [0.0, 1.0]
        double ntlx = tlx / maxThumb;
        double ntly = tly / maxThumb;

        p.rx() += (qint32)(square(ntlx) * step);
        p.ry() -= (qint32)(square(ntly) * step);
        WinSys::setMouseCursorPos(p);
    } else {
         controller.m_mouseAcceleration.setAccelerationHint(0);
    }
}

void ControllerThread::triggerMouseWheel(Controller &controller)
{
    if (!m_enabled || m_keyboard->show())
        return;

    const XINPUT_GAMEPAD &gamepad = controller.m_state[Controller::CURRENT_STATE].Gamepad;

    double tly;
    quint32 deadZone;

    if (m_action->find(Action::ControllerAnalogAction::MouseWheel) == Action::ControllerAnalog::Left) {
        deadZone = m_settings->leftJoystickDeadZone();
        tly = gamepad.sThumbLY;
    }
    else {
        deadZone = m_settings->rightJoystickDeadZone();
        tly = gamepad.sThumbRY;
    }

    // compute magnitude
    double mg = abs(tly);

    if (mg > deadZone) {
        double maxThumb = 32767 - deadZone;
        double speed = 30 * (m_settings->mouseScrollSpeed() + 1.0);

        // normalized tly between [0.0, 1.0]
        double ntly = tly / maxThumb;

        qint32 scroll = (qint32)(square(ntly) * speed);
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

void ControllerThread::handleKeyboard(Controller& controller)
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
    double tlx;
    if (m_action->find(Action::ControllerAnalogAction::KeyboardScroll) == Action::ControllerAnalog::Left) {
        deadZone = m_settings->leftJoystickDeadZone();
        tlx = gamepad.sThumbLX;
    }
    else {
        deadZone = m_settings->rightJoystickDeadZone();
        tlx = gamepad.sThumbRX;
    }

    // compute magnitude
    double mg = abs(tlx);

    if (mg > deadZone) {
        double maxThumb = 32767 - deadZone;
        double speed = (m_settings->mouseScrollSpeed() + 1.0) / 20;

        // normalized tlx between [0.0, 1.0]
        double ntlx = tlx / maxThumb;

        m_keyboard->incCharacterIndex(square(ntlx) * speed);
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

            handleKeyboard(controller);
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
