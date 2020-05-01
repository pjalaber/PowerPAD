#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool runOnStartup READ runOnStartup WRITE setRunOnStartup NOTIFY runOnStartupChanged)
    Q_PROPERTY(bool playSoundsOnDisable READ playSoundsOnDisable WRITE setPlaySoundsOnDisable NOTIFY playSoundsOnDisableChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(double mouseSpeed READ mouseSpeed WRITE setMouseSpeed NOTIFY mouseSpeedChanged)
    Q_PROPERTY(quint32 leftJoystickDeadZone READ leftJoystickDeadZone WRITE setLeftJoystickDeadZone NOTIFY leftJoystickDeadZoneChanged)
    Q_PROPERTY(quint32 rightJoystickDeadZone READ rightJoystickDeadZone WRITE setRightJoystickDeadZone NOTIFY rightJoystickDeadZoneChanged)

public:
    static const bool RUN_ON_STARTUP_DEFAULT;

    static const QString PLAY_SOUNDS_ON_DISABLE_KEY;
    static const bool PLAY_SOUNDS_ON_DISABLE_SYSTEM;
    static const bool PLAY_SOUNDS_ON_DISABLE_DEFAULT;

    static const QString LANGUAGE_KEY;
    static const QString LANGUAGE_STRING_SYSTEM;
    static const QString LANGUAGE_STRING_DEFAULT;

    static const QString MOUSE_SPEED_KEY;
    static const double MOUSE_SPEED_DEFAULT, MOUSE_SPEED_MIN, MOUSE_SPEED_MAX;

    static const QString LEFT_JOYSTICK_DEADZONE_KEY;
    static const QString RIGHT_JOYSTICK_DEADZONE_KEY;
    static const quint32 JOYSTICK_DEADZONE_DEFAULT, JOYSTICK_DEADZONE_MIN, JOYSTICK_DEADZONE_MAX;

private:
    QSettings m_settings, m_winStartupSettings;
    double m_mouseSpeed;
    bool m_runOnStartup;
    bool m_playSoundsOnDisable;
    quint32 m_leftJoystickDeadZone;
    quint32 m_rightJoystickDeadZone;
    QString m_language;

protected:
    static double mouseSpeedNormalize(double mouseSpeed);
    static quint32 joystickNormalize(quint32 joystickSpeed);

public:
    explicit Settings(QObject *parent = nullptr);
    static Settings* instance();

    void setRunOnStartup(const bool &runOnStartup);
    bool runOnStartup();

    void setPlaySoundsOnDisable(const bool &playSoundsOnDisable);
    bool playSoundsOnDisable();

    void setLanguage(const QString &language);
    QString language();

    void setLeftJoystickDeadZone(const quint32 &leftJoystickDeadZone);
    quint32 leftJoystickDeadZone();
    Q_INVOKABLE void setLeftJoystickDeadZoneDefault();

    void setRightJoystickDeadZone(const quint32 &rightJoystickDeadZone);
    quint32 rightJoystickDeadZone();
    Q_INVOKABLE void setRightJoystickDeadZoneDefault();

    void setMouseSpeed(const double &mouseSpeed);
    double mouseSpeed();
    Q_INVOKABLE void setMouseSpeedDefault();

    Q_INVOKABLE void commit();
    Q_INVOKABLE void revert();

signals:
    void mouseSpeedChanged();
    void runOnStartupChanged();
    void playSoundsOnDisableChanged();
    void languageChanged();
    void leftJoystickDeadZoneChanged();
    void rightJoystickDeadZoneChanged();
};

#endif // SETTINGS_H
