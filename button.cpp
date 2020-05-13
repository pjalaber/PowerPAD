#include "button.h"

ButtonTimer::ButtonTimer(): m_buttonState(ButtonState::None), m_timer()
{
}

void ButtonTimer::start(ButtonState buttonState)
{
  m_buttonState = buttonState;
  m_timer.start();
}

void ButtonTimer::invalidate()
{
    m_buttonState = ButtonState::None;
    m_timer.invalidate();
}

bool ButtonTimer::stateHasChangedOrExpired(ButtonState buttonState, quint32 expireDelayMs)
{
    return (m_buttonState != buttonState || !m_timer.isValid() || m_timer.elapsed() >= expireDelayMs);
}


ButtonCombo::ButtonCombo(ButtonState targetState, quint32 pressDelayMs) : m_targetState(targetState),
    m_button1Timer(), m_button2Timer(), m_pressDelayMs(pressDelayMs)
{
}

void ButtonCombo::updateState(ButtonState button1State, ButtonState button2State)
{
    if (button1State == m_targetState && m_button1Timer.stateHasChangedOrExpired(button1State, m_pressDelayMs))
        m_button1Timer.start(button1State);

    if (button2State == m_targetState && m_button2Timer.stateHasChangedOrExpired(button2State, m_pressDelayMs))
        m_button2Timer.start(button2State);
}

void ButtonCombo::clear()
{
    m_button1Timer.invalidate();
    m_button2Timer.invalidate();
}

bool ButtonCombo::isComboOn()
{
    return (!m_button1Timer.stateHasChangedOrExpired(m_targetState, m_pressDelayMs) &&
            !m_button2Timer.stateHasChangedOrExpired(m_targetState, m_pressDelayMs));
}
