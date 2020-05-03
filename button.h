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

class ButtonStateTimer
{
protected:
    ButtonState m_buttonState;
    QElapsedTimer m_timer;

public:
    ButtonStateTimer();
    bool isStillActive(ButtonState buttonState);
    void startActive(ButtonState buttonState);
    void clear();
};


#endif // BUTTON_H
