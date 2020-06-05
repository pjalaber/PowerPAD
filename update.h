#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QProcess>

class Update : public QObject
{
    Q_OBJECT
private:
    QProcess m_process;
    QString m_availableVersion;
public:
    explicit Update(QObject *parent = nullptr);
    static Update* instance();
    Q_INVOKABLE void startCheckUpdate(void);

signals:
    void checkUpdateStarted();
    void checkUpdateFinishedNoUpdate();
    void checkUpdateFinishedUpdateAvailable(const QString currentVersion, const QString newVersion);
    void checkUpdateErrorOccured(QProcess::ProcessError);
};

#endif // UPDATE_H
