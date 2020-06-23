#include <QDebug>
#include "mouseacceleration.h"

MouseAcceleration::MouseAcceleration(): m_state(MouseAccelerationNone), m_timer()
{
}

bool MouseAcceleration::isAccelerationOn()
{
    return m_state != MouseAccelerationNone;
}

void MouseAcceleration::setAccelerationHint(double magnitude)
{
    switch (m_state) {
    case MouseAccelerationNone:
        if (magnitude >= 1) {
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
        if (magnitude == 0) {
            m_timer.start();
            m_state = MouseAccelerationGrace;
        }
        break;
    case MouseAccelerationGrace:
        if (magnitude == 1) {
            m_state = MouseAccelerationOn;
        } else if (magnitude == 0 || m_timer.elapsed() >= 200) {
            m_timer.invalidate();
            m_state = MouseAccelerationNone;
        }
        break;
    }   
}
