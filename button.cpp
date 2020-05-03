#include "button.h"

ButtonHistory::ButtonHistory() : m_buttonState(ButtonState::None), m_timer()
{
}

bool ButtonHistory::isStillActive(ButtonState buttonState)
{
    if (m_buttonState != buttonState || !m_timer.isValid())
        return false;
    return (m_timer.elapsed() <= 200);
}

void ButtonHistory::startActive(ButtonState buttonState)
{
    m_buttonState = buttonState;
    m_timer.start();
}

void ButtonHistory::clear()
{
    m_buttonState = ButtonState::None;
    m_timer.invalidate();
}
