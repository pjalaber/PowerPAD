#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double mouseSpeed READ mouseSpeed WRITE setMouseSpeed NOTIFY mouseSpeedChanged)
    Q_PROPERTY(bool runOnStartup READ runOnStartup WRITE setRunOnStartup NOTIFY runOnStartupChanged)
private:
    static const QString MOUSE_SPEED_STRING;
    static const double MOUSE_SPEED_DEFAULT, MOUSE_SPEED_MIN, MOUSE_SPEED_MAX;

    static const QString RUN_ON_STARTUP_STRING;
    static const bool RUN_ON_STARTUP_DEFAULT;

    static double mouseSpeedNormalize(double mouseSpeed);
private:
    QSettings m_settings;
    double m_mouseSpeed;
    bool m_runOnStartup;
public:
    explicit Settings(QObject *parent = nullptr);
    static Settings* instance();

    void setMouseSpeed(const double &mouseSpeed);
    double mouseSpeed();

    void setRunOnStartup(bool runOnStartup);
    bool runOnStartup();

    Q_INVOKABLE void commit();
    Q_INVOKABLE void revert();

signals:
    void mouseSpeedChanged();
    void runOnStartupChanged();
};

#endif // SETTINGS_H
