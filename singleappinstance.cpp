#include "singleappinstance.h"

SingleAppInstance::SingleAppInstance(QObject *parent) : QObject(parent),
    m_sem("PowerPADSem", 1), m_shm("PowerPADShm")
{
}

bool SingleAppInstance::isRunning()
{
    m_sem.acquire();

    bool isRunning = m_shm.attach();
    if (isRunning)
        m_shm.detach();
    else
        m_shm.create(1);

    m_sem.release();

    return isRunning;
}
