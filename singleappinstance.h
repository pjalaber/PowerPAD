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
private:
    explicit SingleAppInstance(QObject *parent = nullptr);
public:
    static SingleAppInstance* instance();
    bool isRunning();
    void restart();
};

#endif // SINGLEINSTANCE_H
