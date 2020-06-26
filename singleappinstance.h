#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QObject>
#include <QSystemSemaphore>
#include <QSharedMemory>

class SingleAppInstance : public QObject
{
    Q_OBJECT
protected:
    QSystemSemaphore m_sem;
    QSharedMemory m_shm;
public:
    explicit SingleAppInstance(QObject *parent = nullptr);
    bool isRunning();
};

#endif // SINGLEINSTANCE_H
