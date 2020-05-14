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
    quint32 m_expireDelayMsec;

public:
    ButtonTimer();
    void start(ButtonState buttonState, quint32 expireDelayMsec);
    bool isValid();
    void invalidate();
    bool hasExpired();
    bool stateHasChangedOrExpired(ButtonState buttonState);
};


class ButtonCombo
{
protected:
    ButtonState m_targetState;
    ButtonTimer m_button1Timer, m_button2Timer;
    quint32 m_pressDelayMs;

public:
    ButtonCombo() = delete;
    ButtonCombo(ButtonState targetState);
    void updateState(ButtonState button1State, ButtonState button2State, quint32 pressDelayMsec);
    void clear();
    bool isComboOn();
};

#endif // BUTTON_H
