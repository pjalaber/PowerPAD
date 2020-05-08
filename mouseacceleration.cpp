#include "mouseacceleration.h"

MouseAcceleration::MouseAcceleration(): m_state(MouseAccelerationNone), m_timer()
{
}

bool MouseAcceleration::isAccelerationOn()
{
    return m_state != MouseAccelerationNone;
}

void MouseAcceleration::setAccelerationHint(bool hint)
{
    switch (m_state) {
    case MouseAccelerationNone:
        if (hint) {
            if (!m_timer.isValid())
                m_timer.start();
            else if (m_timer.elapsed() >= 400) {
                m_timer.invalidate();
                m_state = MouseAccelerationOn;
            }
        }
        else
            m_timer.invalidate();
        break;
    case MouseAccelerationOn:
        if (!hint) {
            m_timer.start();
            m_state = MouseAccelerationGrace;
        }
        break;
    case MouseAccelerationGrace:
        if (hint) {
            m_state = MouseAccelerationOn;
        } else if (m_timer.elapsed() >= 200) {
            m_timer.invalidate();
            m_state = MouseAccelerationNone;
        }
        break;
    }
}
