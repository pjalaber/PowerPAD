#pragma once

#include <QThread>
#include <QElapsedTimer>
#include <QLibrary>
#include <QQmlEngine>
#include <xinput.h>
#include "mouseacceleration.h"
#include "button.h"
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
    QElapsedTimer m_accelerationTimer;
    ButtonStateTimer m_startButtonStateTimer;
    ButtonStateTimer m_backButtonStateTimer;
    MouseAcceleration m_mouseAcceleration;
    Controller();
    bool buttonIsDown(DWORD button, quint32 state) const;
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
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(quint32 connectedCount READ connectedCount NOTIFY connectedCountChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
public:
    enum Status {
        StatusOK,
        StatusXInputLibraryNotFound,
        StatusXInputSymbolNotFound,
        StatusShell32NotFound,
        StatusShell32SymbolNotFound
    };
    Q_ENUM(Status)

private:
    QLibrary m_xinputLib, m_shellLib;

protected:
	Controller m_controller[XUSER_MAX_COUNT];
    quint32 m_leftThumbDeadZone;
	bool m_shouldStop;
    bool m_enabled;
    quint32 m_connectedCount;
    Status m_status;
    Settings *m_settings;
    Keyboard *m_keyboard;

    static qint32 getNormDeadZone(SHORT value, SHORT deadZone);
    static void sendUnicodeKeyDown(quint16 unicodeKey);
    static void sendKeyUp();

    void updateMousePosition(Controller& controller, double delta);
    void triggerMouseWheel(Controller &controller);
    void triggerMouseButton(const Controller& controller);
    void manageKeyboard(Controller& controller, double delta);

private:
    void run();

public:
    static const quint32 FPS = 60;
    static const quint32 SPEED = 180; // * 10 * pixel per second
    static constexpr double FRAME_DURATION = 1000.0 / FPS;
    ControllerThread();
    static ControllerThread* instance();
    bool start();
    void stop();

    bool enabled();
    void setEnabled(bool enabled);

    quint32 connectedCount();
    void setConnectedCount(quint32 connectedCount);

    Status status();
    void setStatus(Status status);

signals:
    void enabledChanged();
    void connectedCountChanged();
    void statusChanged();
};

