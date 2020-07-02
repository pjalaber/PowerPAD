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
    Q_PROPERTY(double mouseAcceleration READ mouseAcceleration WRITE setMouseAcceleration NOTIFY mouseAccelerationChanged)
    Q_PROPERTY(double mouseScrollSpeed READ mouseScrollSpeed WRITE setMouseScrollSpeed NOTIFY mouseScrollSpeedChanged)
    Q_PROPERTY(quint32 leftJoystickDeadZone READ leftJoystickDeadZone WRITE setLeftJoystickDeadZone NOTIFY leftJoystickDeadZoneChanged)
    Q_PROPERTY(quint32 rightJoystickDeadZone READ rightJoystickDeadZone WRITE setRightJoystickDeadZone NOTIFY rightJoystickDeadZoneChanged)

public:
    static const QString RUN_ON_STARTUP_LNK_FILENAME;
    static const bool RUN_ON_STARTUP_DEFAULT;

    static const QString PLAY_SOUNDS_ON_DISABLE_KEY;
    static const bool PLAY_SOUNDS_ON_DISABLE_SYSTEM;
    static const bool PLAY_SOUNDS_ON_DISABLE_DEFAULT;

    static const QString LANGUAGE_KEY;
    static const QString LANGUAGE_STRING_SYSTEM;
    static const QString LANGUAGE_STRING_DEFAULT;

    static const QString MOUSE_SPEED_KEY;
    static const double MOUSE_SPEED_DEFAULT, MOUSE_SPEED_MIN, MOUSE_SPEED_MAX;

    static const QString MOUSE_ACCELERATION_KEY;
    static const double MOUSE_ACCELERATION_DEFAULT, MOUSE_ACCELERATION_MIN, MOUSE_ACCELERATION_MAX;

    static const QString MOUSE_SCROLL_SPEED_KEY;
    static const double MOUSE_SCROLL_SPEED_DEFAULT, MOUSE_SCROLL_SPEED_MIN, MOUSE_SCROLL_SPEED_MAX;

    static const QString LEFT_JOYSTICK_DEADZONE_KEY;
    static const QString RIGHT_JOYSTICK_DEADZONE_KEY;
    static const quint32 JOYSTICK_DEADZONE_DEFAULT, JOYSTICK_DEADZONE_MIN, JOYSTICK_DEADZONE_MAX;

private:
    QSettings m_settings;
    double m_mouseSpeed;
    double m_mouseAcceleration;
    double m_mouseScrollSpeed;
    bool m_runOnStartup;
    bool m_playSoundsOnDisable;
    quint32 m_leftJoystickDeadZone;
    quint32 m_rightJoystickDeadZone;
    QString m_language;

protected:
    static double mouseSpeedNormalize(double mouseSpeed);
    static double mouseScrollSpeedNormalize(double mouseScrollSpeed);
    static double mouseAccelerationNormalize(double mouseAcceleration);
    static quint32 joystickNormalize(quint32 joystickSpeed);

private:
    explicit Settings(QObject *parent = nullptr);
public:
    static Settings* instance();

    void setRunOnStartup(bool runOnStartup);
    bool runOnStartup();

    void setPlaySoundsOnDisable(bool playSoundsOnDisable);
    bool playSoundsOnDisable();

    void setLanguage(const QString &language);
    QString language();

    void setLeftJoystickDeadZone(quint32 leftJoystickDeadZone);
    quint32 leftJoystickDeadZone();

    void setRightJoystickDeadZone(quint32 rightJoystickDeadZone);
    quint32 rightJoystickDeadZone();

    Q_INVOKABLE quint32 joystickDeadZoneDefault();
    Q_INVOKABLE quint32 joystickDeadZoneMin();
    Q_INVOKABLE quint32 joystickDeadZoneMax();

    void setMouseSpeed(double mouseSpeed);
    double mouseSpeed();
    Q_INVOKABLE double mouseSpeedDefault();
    Q_INVOKABLE double mouseSpeedMin();
    Q_INVOKABLE double mouseSpeedMax();

    void setMouseAcceleration(double mouseAcceleration);
    double mouseAcceleration();
    Q_INVOKABLE double mouseAccelerationDefault();
    Q_INVOKABLE double mouseAccelerationMin();
    Q_INVOKABLE double mouseAccelerationMax();

    void setMouseScrollSpeed(double mouseScrollSpeed);
    double mouseScrollSpeed();
    Q_INVOKABLE double mouseScrollSpeedDefault();
    Q_INVOKABLE double mouseScrollSpeedMin();
    Q_INVOKABLE double mouseScrollSpeedMax();

    Q_INVOKABLE void commit();
    Q_INVOKABLE void revert();

signals:
    void mouseSpeedChanged();
    void mouseAccelerationChanged();
    void mouseScrollSpeedChanged();
    void runOnStartupChanged();
    void playSoundsOnDisableChanged();
    void languageChanged();
    void leftJoystickDeadZoneChanged();
    void rightJoystickDeadZoneChanged();
};

#endif // SETTINGS_H
