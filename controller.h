#pragma once

#include <QThread>
#include <QElapsedTimer>
#include <QLibrary>
#include <QQmlEngine>
#include <xinput.h>

enum class ButtonState
{
    Down, StillDown, Up, None
};

class ButtonHistory
{
public:
    ButtonState m_buttonState;
    QElapsedTimer m_timer;

    ButtonHistory() : m_buttonState(ButtonState::None), m_timer()
    {
    }
	bool IsStillActive(ButtonState buttonState)
	{
        if (m_buttonState != buttonState || !m_timer.isValid())
			return false;
        return (m_timer.elapsed() <= 200);
	}
	void StartActive(ButtonState buttonState)
	{
		m_buttonState = buttonState;
        m_timer.start();
	}
	void Clear()
	{
        m_buttonState = ButtonState::None;
        m_timer.invalidate();
    }
};

class Controller
{
protected:
    bool m_connected;
    QElapsedTimer m_connectCheckTimer;
public:
    static const quint32 CURRENT_STATE = 0;
    static const quint32 PREVIOUS_STATE = 1;
	XINPUT_STATE m_state[2];
	ButtonHistory m_startButtonHistory;
	ButtonHistory m_backButtonHistory;
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
private:
    QLibrary m_lib;

protected:
	Controller m_controller[XUSER_MAX_COUNT];
    quint32 m_leftThumbDeadZone;
	bool m_shouldStop;
    bool m_enabled;
    quint32 m_connectedCount;

    static qint32 getNormDeadZone(SHORT value, SHORT deadZone);
    void updateMousePosition(const Controller& controller, double delta);
    void triggerMouseEvent(const Controller& controller);

private:
    void run();

public:
    static const quint32 JOYSTICK_MAX_VALUE = 32767;
    static const quint32 DEF_JOYSTICK_DEADZONE = (15 * JOYSTICK_MAX_VALUE / 100);
    static const quint32 FPS = 60;
    static const quint32 SPEED= 800; // pixel per second
    static constexpr double FRAME_DURATION = 1000.0 / FPS;
    ControllerThread();
    static ControllerThread* instance();
    bool start();
    void stop();

    bool enabled();
    void setEnabled(const bool &enabled);

    quint32 connectedCount();
    void setConnectedCount(const quint32 &connectedCount);

signals:
    void enabledChanged();
    void connectedCountChanged();
};

