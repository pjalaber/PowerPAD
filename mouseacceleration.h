#ifndef MOUSEACCELERATION_H
#define MOUSEACCELERATION_H

#include <QElapsedTimer>

class MouseAcceleration
{
protected:
    enum {MouseAccelerationNone, MouseAccelerationOn, MouseAccelerationGrace} m_state;
    QElapsedTimer m_timer;

public:
    MouseAcceleration();
    bool isAccelerationOn();
    void setAccelerationHint(double magnitude);
};


#endif // MOUSEACCELERATION_H
