#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QRect>
#include <QSettings>
#include <windows.h>

#define POWERPAD_STRINGIFY(s) POWERPAD_STRINGIFY2(s)
#define POWERPAD_STRINGIFY2(s) #s

class Helper : public QObject
{
    Q_OBJECT
protected:
    QSettings m_winSystemSoundSettings;
    HWND m_foregroundWindow;
private:
    explicit Helper(QObject *parent = nullptr);
public:
    static Helper* instance();

    Q_INVOKABLE void restartApp();
    Q_INVOKABLE QRect computeBestWindowRect(const QString &screenName, const QRect &r,
                                            qint32 windowWidth, qint32 windowHeight,
                                            qint32 hMargin = 0, qint32 vMargin = 0);
    Q_INVOKABLE QString getAboutText();
    Q_INVOKABLE QString getDeviceConnectSoundFilename();
    Q_INVOKABLE QString getDeviceDisconnectSoundFilename();
    Q_INVOKABLE void saveForegroundWindow();
    Q_INVOKABLE void restoreForegroundWindow();

private slots:
    void logicalDotsPerInchChanged(qreal dpi);
};

#endif // HELPER_H
