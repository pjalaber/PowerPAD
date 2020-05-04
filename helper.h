#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QRect>
#include <QSettings>

class Helper : public QObject
{
    Q_OBJECT
protected:
    QSettings m_winSystemSoundSettings;
public:
    explicit Helper(QObject *parent = nullptr);
    static Helper* instance();

    Q_INVOKABLE void restartApp();

    Q_INVOKABLE QRect computeBestWindowRect(const QString &screenName, const QRect &r,
                                            qint32 windowWidth, qint32 windowHeight);
    Q_INVOKABLE QString getAboutText();
    Q_INVOKABLE QString getDeviceConnectSoundFilename();
    Q_INVOKABLE QString getDeviceDisconnectSoundFilename();

};

#endif // HELPER_H
