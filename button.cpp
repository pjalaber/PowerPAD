#include "button.h"

ButtonTimer::ButtonTimer(): m_buttonState(ButtonState::None), m_timer(), m_expireDelayMsec(0)
{
}

void ButtonTimer::start(ButtonState buttonState, quint32 expireDelayMsec)
{
  m_buttonState = buttonState;
  m_expireDelayMsec = expireDelayMsec;
  m_timer.start();
}

bool ButtonTimer::isValid()
{
    return m_timer.isValid();
}

void ButtonTimer::invalidate()
{
    m_buttonState = ButtonState::None;
    m_timer.invalidate();
}

bool ButtonTimer::hasExpired()
{
    return m_timer.elapsed() >= m_expireDelayMsec;
}

bool ButtonTimer::stateHasChangedOrExpired(ButtonState buttonState)
{
    return (m_buttonState != buttonState || !isValid() || hasExpired());
}

ButtonCombo::ButtonCombo(ButtonState targetState) : m_targetState(targetState),
    m_button1Timer(), m_button2Timer()
{
}

void ButtonCombo::updateState(ButtonState button1State, ButtonState button2State, quint32 pressDelayMsec)
{
    if (button1State == m_targetState && m_button1Timer.stateHasChangedOrExpired(button1State))
        m_button1Timer.start(button1State, pressDelayMsec);

    if (button2State == m_targetState && m_button2Timer.stateHasChangedOrExpired(button2State))
        m_button2Timer.start(button2State, pressDelayMsec);
}

void ButtonCombo::clear()
{
    m_button1Timer.invalidate();
    m_button2Timer.invalidate();
}

bool ButtonCombo::isComboOn()
{
    return (!m_button1Timer.stateHasChangedOrExpired(m_targetState) &&
            !m_button2Timer.stateHasChangedOrExpired(m_targetState));
}

