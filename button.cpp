#include "button.h"

ButtonStateTimer::ButtonStateTimer() : m_buttonState(ButtonState::None), m_timer()
{
}

bool ButtonStateTimer::isStillActive(ButtonState buttonState)
{
    if (m_buttonState != buttonState || !m_timer.isValid())
        return false;
    return (m_timer.elapsed() <= 200);
}

void ButtonStateTimer::startActive(ButtonState buttonState)
{
    m_buttonState = buttonState;
    m_timer.start();
}

void ButtonStateTimer::clear()
{
    m_buttonState = ButtonState::None;
    m_timer.invalidate();
}
