#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double mouseSpeed READ mouseSpeed WRITE setMouseSpeed NOTIFY mouseSpeedChanged)
    Q_PROPERTY(bool runOnStartup READ runOnStartup WRITE setRunOnStartup NOTIFY runOnStartupChanged)
    Q_PROPERTY(bool playSoundsOnDisable READ playSoundsOnDisable WRITE setPlaySoundsOnDisable NOTIFY playSoundsOnDisableChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
public:
    static const QString MOUSE_SPEED_KEY;
    static const double MOUSE_SPEED_DEFAULT, MOUSE_SPEED_MIN, MOUSE_SPEED_MAX;

    static const bool RUN_ON_STARTUP_DEFAULT;

    static const QString PLAY_SOUNDS_ON_DISABLE_KEY;
    static const bool PLAY_SOUNDS_ON_DISABLE_SYSTEM;
    static const bool PLAY_SOUNDS_ON_DISABLE_DEFAULT;

    static const QString LANGUAGE_KEY;
    static const QString LANGUAGE_STRING_SYSTEM;
    static const QString LANGUAGE_STRING_DEFAULT;

private:
    QSettings m_settings, m_winStartupSettings;
    double m_mouseSpeed;
    bool m_runOnStartup;
    bool m_playSoundsOnDisable;
    QString m_language;

protected:
    static double mouseSpeedNormalize(double mouseSpeed);

public:
    explicit Settings(QObject *parent = nullptr);
    static Settings* instance();

    void setMouseSpeed(const double &mouseSpeed);
    double mouseSpeed();

    void setRunOnStartup(const bool &runOnStartup);
    bool runOnStartup();

    void setPlaySoundsOnDisable(const bool &playSoundsOnDisable);
    bool playSoundsOnDisable();

    void setLanguage(const QString &language);
    QString language();

    Q_INVOKABLE void setMouseSpeedDefault();
    Q_INVOKABLE void commit();
    Q_INVOKABLE void revert();

signals:
    void mouseSpeedChanged();
    void runOnStartupChanged();
    void playSoundsOnDisableChanged();
    void languageChanged();
};

#endif // SETTINGS_H
