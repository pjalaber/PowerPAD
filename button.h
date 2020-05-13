#ifndef BUTTON_H
#define BUTTON_H

#include <QElapsedTimer>

enum class ButtonState
{
    Down,
    StillDown,
    Up,
    None
};

class ButtonTimer
{
protected:
    ButtonState m_buttonState;
    QElapsedTimer m_timer;

public:
    ButtonTimer();
    void start(ButtonState buttonState);
    void invalidate();
    bool stateHasChangedOrExpired(ButtonState buttonState, quint32 expireDelayMs);
};


class ButtonCombo
{
protected:
    ButtonState m_targetState;
    ButtonTimer m_button1Timer, m_button2Timer;
    quint32 m_pressDelayMs;

public:
    ButtonCombo() = delete;
    ButtonCombo(ButtonState targetState, quint32 pressDelayMs);
    void updateState(ButtonState button1State, ButtonState button2State);
    void clear();
    bool isComboOn();
};

#endif // BUTTON_H
