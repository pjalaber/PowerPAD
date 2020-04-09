#pragma once

#include <QThread>
#include <QElapsedTimer>
#include <QLibrary>
#include <xinput.h>

enum class ButtonState
{
	None, Down, Up
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
public:
    static const quint32 CURRENT_STATE = 0;
    static const quint32 PREVIOUS_STATE = 1;
	bool m_connected;
	bool m_controlMouse;
	XINPUT_STATE m_state[2];
    QElapsedTimer m_lastStateCheckElapsedTimer;
	ButtonHistory m_startButtonHistory;
	ButtonHistory m_backButtonHistory;
	Controller();
    bool buttonIsDown(DWORD button, UINT state) const;
    ButtonState getButtonState(DWORD button) const;
    void saveCurrentState(void);
};

class ControllerThread: public QThread
{
private:
    QLibrary m_lib;

protected:
	Controller m_controller[XUSER_MAX_COUNT];
    quint16 m_leftThumbDeadZone;
    quint16 m_horiz_speed, m_vert_speed;
	bool m_shouldStop;

    static INT getNormDeadZone(SHORT value, SHORT deadZone);
    void updateMousePosition(const Controller& controller);
    void triggerMouseEvent(const Controller& controller);

private:
    void run();

public:
	static const USHORT JOYSTICK_MAX_VALUE = 32767;
	static const USHORT DEF_HORIZ_SPEED = 30;
	static const USHORT DEF_VERT_SPEED = 30;
	static const USHORT DEF_JOYSTICK_DEADZONE = (24 * JOYSTICK_MAX_VALUE / 100);
    static const UINT MOUSE_FPS = 25;
    ControllerThread();
    bool start();
    void stop();
};

