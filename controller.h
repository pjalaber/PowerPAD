#pragma once

#include <QThread>
#include <QElapsedTimer>
#include <QLibrary>
#include <QQmlEngine>
#include <xinput.h>
#include "mouseacceleration.h"
#include "button.h"
#include "action.h"
#include "winsys.h"
#include "settings.h"
#include "keyboard.h"

class Controller
{
protected:
    bool m_connected;
    QElapsedTimer m_connectCheckTimer;
public:
    static const quint32 CURRENT_STATE = 0;
    static const quint32 PREVIOUS_STATE = 1;
    XINPUT_STATE m_state[2];
    bool m_accelerationGraceTimeState;
    QElapsedTimer m_keyboardTimer;
    ButtonCombo m_enableDisableButtonCombo;
    MouseAcceleration m_mouseAcceleration;
    ButtonTimer m_repeatTimer;
    Controller();
    bool buttonIsDown(DWORD button, quint32 state) const;    
    bool singleButtonIsDown(DWORD button) const;
    ButtonState getButtonState(DWORD button) const;
    bool readCurrentState(quint32 i);
    void saveCurrentState(void);
    bool isConnected()
    {
        return m_connected;
    }
};

class ControllerThread: public QThread
{
    Q_OBJECT

public:
    enum State
    {
        StateEnabledWithController,
        StateDisabledWithController,
        StateEnabledWithUI,
        StateDisabledWithUI,
        StateEnabledWithFullscreenExit,
        StateDisabledWithFullScreenEnter
    };
    Q_ENUM(State)

    static bool stateIsEnabled(State s);

    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(quint32 connectedCount READ connectedCount NOTIFY connectedCountChanged)

protected:
	Controller m_controller[XUSER_MAX_COUNT];
    quint32 m_leftThumbDeadZone;
	bool m_shouldStop;
    State m_state;
    quint32 m_connectedCount;
    Action *m_action;
    WinSys *m_winsys;
    Settings *m_settings;
    Keyboard *m_keyboard;

    static double square(double value);

    void handleAction(Controller& controller, Action::ControllerButtonAction action);

    void updateMousePosition(Controller& controller, double delta);
    void triggerMouseWheel(Controller &controller);
    void triggerMouseButton(const Controller& controller);
    void handleKeyboard(Controller& controller);
    void handleButtons(Controller &controller);
    void handleComboButtons(Controller& controller);    

private:
    ControllerThread();
    void run();

public:
    static const quint32 FPS = 60;
    static const quint32 SPEED = 180; // * 10 * pixel per second
    static constexpr double FRAME_DURATION = 1000.0 / FPS;
    static ControllerThread* instance();
    void start();
    void stop();

    State state();
    void setState(State state);

    quint32 connectedCount();
    void setConnectedCount(quint32 connectedCount);

signals:
    void stateChanged();
    void connectedCountChanged();
};

