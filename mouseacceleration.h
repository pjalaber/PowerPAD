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
    void setAccelerationHint(bool hint);
};


#endif // MOUSEACCELERATION_H
