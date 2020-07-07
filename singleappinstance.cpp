#include <QProcess>
#include <QCoreApplication>
#include "singleappinstance.h"

SingleAppInstance::SingleAppInstance(QObject *parent) : QObject(parent),
    m_sem("PowerPADSem", 1), m_shm("PowerPADShm")
{
}

SingleAppInstance* SingleAppInstance::instance()
{
    static SingleAppInstance *s = new SingleAppInstance();
    return s;
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

void SingleAppInstance::restart()
{
    m_sem.acquire();
    m_shm.detach();
    m_sem.release();
    QCoreApplication *app = QCoreApplication::instance();
    app->quit();
    QProcess::startDetached(app->arguments()[0], app->arguments());
}


void SingleAppInstance::restartWithAdminRights()
{
    m_sem.acquire();
    m_shm.detach();
    m_sem.release();
    QCoreApplication *app = QCoreApplication::instance();
    app->quit();
    QProcess::startDetached("schtasks.exe", QStringList({"/Run", "/TN", "PowerPAD"}));
}

