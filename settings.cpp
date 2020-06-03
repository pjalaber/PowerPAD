#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>
#include "settings.h"

const QString Settings::RUN_ON_STARTUP_LNK_FILENAME = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
        "/Microsoft/Windows/Start Menu/Programs/Startup/PowerPAD.lnk";

const QString Settings::PLAY_SOUNDS_ON_DISABLE_KEY = "General/PlaySoundsOnDisable";
const bool Settings::PLAY_SOUNDS_ON_DISABLE_DEFAULT = true;

const QString Settings::LANGUAGE_KEY = "General/Language";
const QString Settings::LANGUAGE_STRING_SYSTEM = "SYSTEM";
const QString Settings::LANGUAGE_STRING_DEFAULT = LANGUAGE_STRING_SYSTEM;

const QString Settings::MOUSE_SPEED_KEY = "Mouse/Speed";
const double Settings::MOUSE_SPEED_MIN = 1.0;
const double Settings::MOUSE_SPEED_DEFAULT = 5.0;
const double Settings::MOUSE_SPEED_MAX = 11.0;

const QString Settings::MOUSE_ACCELERATION_KEY = "Mouse/Acceleration";
const double Settings::MOUSE_ACCELERATION_MIN = 1.0;
const double Settings::MOUSE_ACCELERATION_DEFAULT = 2.0;
const double Settings::MOUSE_ACCELERATION_MAX = 6.0;

const QString Settings::MOUSE_SCROLL_SPEED_KEY = "Mouse/ScrollSpeed";
const double Settings::MOUSE_SCROLL_SPEED_MIN = 1.0;
const double Settings::MOUSE_SCROLL_SPEED_DEFAULT = 5.0;
const double Settings::MOUSE_SCROLL_SPEED_MAX = 11.0;

const QString Settings::LEFT_JOYSTICK_DEADZONE_KEY = "Controller/LeftJoystickDeadZone";
const QString Settings::RIGHT_JOYSTICK_DEADZONE_KEY = "Controller/RightJoystickDeadZone";
const quint32 Settings::JOYSTICK_DEADZONE_MIN = 2000;
const quint32 Settings::JOYSTICK_DEADZONE_DEFAULT = 3500;
const quint32 Settings::JOYSTICK_DEADZONE_MAX = 12000;

double Settings::mouseSpeedNormalize(double mouseSpeed)
{
    return qMin(MOUSE_SPEED_MAX, qMax(MOUSE_SPEED_MIN, mouseSpeed));
}

double Settings::mouseScrollSpeedNormalize(double mouseScrollSpeed)
{
    return qMin(MOUSE_SCROLL_SPEED_MAX, qMax(MOUSE_SCROLL_SPEED_MIN, mouseScrollSpeed));
}

double Settings::mouseAccelerationNormalize(double mouseAcceleration)
{
    return qMin(MOUSE_ACCELERATION_MAX, qMax(MOUSE_ACCELERATION_MIN, mouseAcceleration));
}

quint32 Settings::joystickNormalize(quint32 joystickSpeed)
{
    return qMin(JOYSTICK_DEADZONE_MAX, qMax(JOYSTICK_DEADZONE_MIN, joystickSpeed));
}

Settings::Settings(QObject *parent) : QObject(parent),
    m_settings("Tekit", "PowerPAD")
{
    revert();
}

Settings* Settings::instance()
{
    static Settings *s = new Settings();
    return s;
}

void Settings::setMouseSpeed(double mouseSpeed)
{
    if (m_mouseSpeed == mouseSpeed)
        return;
    m_mouseSpeed = mouseSpeedNormalize(mouseSpeed);
    emit mouseSpeedChanged();
}

double Settings::mouseSpeed()
{
    return m_mouseSpeed;
}

double Settings::mouseSpeedDefault()
{
    return MOUSE_SPEED_DEFAULT;
}

double Settings::mouseSpeedMin()
{
    return MOUSE_SPEED_MIN;
}

double Settings::mouseSpeedMax()
{
    return MOUSE_SPEED_MAX;
}

void Settings::setMouseAcceleration(double mouseAcceleration)
{
    if (m_mouseAcceleration == mouseAcceleration)
        return;
    m_mouseAcceleration = mouseAccelerationNormalize(mouseAcceleration);
    emit mouseAccelerationChanged();
}

double Settings::mouseAcceleration()
{
    return m_mouseAcceleration;
}

double Settings::mouseAccelerationDefault()
{
    return MOUSE_ACCELERATION_DEFAULT;
}

double Settings::mouseAccelerationMin()
{
    return MOUSE_ACCELERATION_MIN;
}

double Settings::mouseAccelerationMax()
{
    return MOUSE_ACCELERATION_MAX;
}

void Settings::setMouseScrollSpeed(double mouseScrollSpeed)
{
    if (m_mouseScrollSpeed == mouseScrollSpeed)
        return;
    m_mouseScrollSpeed = mouseScrollSpeedNormalize(mouseScrollSpeed);
    emit mouseScrollSpeedChanged();
}

double Settings::mouseScrollSpeed()
{
    return m_mouseScrollSpeed;
}

double Settings::mouseScrollSpeedDefault()
{
    return MOUSE_SCROLL_SPEED_DEFAULT;
}

double Settings::mouseScrollSpeedMin()
{
    return MOUSE_SCROLL_SPEED_MIN;
}

double Settings::mouseScrollSpeedMax()
{
    return MOUSE_SCROLL_SPEED_MAX;
}

void Settings::setRunOnStartup(bool runOnStartup)
{
    if (m_runOnStartup != runOnStartup) {
        m_runOnStartup = runOnStartup;
        emit runOnStartupChanged();
    }
}

bool Settings::runOnStartup()
{
    return m_runOnStartup;
}

void Settings::setPlaySoundsOnDisable(bool playSoundsOnDisable)
{
    if (m_playSoundsOnDisable != playSoundsOnDisable) {
        m_playSoundsOnDisable = playSoundsOnDisable;
        emit playSoundsOnDisableChanged();
    }
}

bool Settings::playSoundsOnDisable()
{
    return m_playSoundsOnDisable;
}

void Settings::setLanguage(const QString &language)
{
    if (m_language != language) {
        m_language = language;
        emit languageChanged();
    }
}

QString Settings::language()
{
    return m_language;
}

void Settings::setLeftJoystickDeadZone(quint32 leftJoystickDeadZone)
{
    if (m_leftJoystickDeadZone != leftJoystickDeadZone)
    {
        m_leftJoystickDeadZone = leftJoystickDeadZone;
        emit leftJoystickDeadZoneChanged();
    }
}

quint32 Settings::leftJoystickDeadZone()
{
    return m_leftJoystickDeadZone;
}

void Settings::setRightJoystickDeadZone(quint32 rightJoystickDeadZone)
{
    if (m_rightJoystickDeadZone != rightJoystickDeadZone)
    {
        m_rightJoystickDeadZone = rightJoystickDeadZone;
        emit rightJoystickDeadZoneChanged();
    }
}

quint32 Settings::rightJoystickDeadZone()
{
    return m_rightJoystickDeadZone;
}

quint32 Settings::joystickDeadZoneDefault()
{
    return JOYSTICK_DEADZONE_DEFAULT;
}

quint32 Settings::joystickDeadZoneMin()
{
    return JOYSTICK_DEADZONE_MIN;
}

quint32 Settings::joystickDeadZoneMax()
{
    return JOYSTICK_DEADZONE_MAX;
}

void Settings::commit()
{
    qInfo() << QCoreApplication::applicationFilePath() << " " << RUN_ON_STARTUP_LNK_FILENAME;
    if (m_runOnStartup)
        QFile::link(QCoreApplication::applicationFilePath(), RUN_ON_STARTUP_LNK_FILENAME);
    else
        QFile::remove(RUN_ON_STARTUP_LNK_FILENAME);

    m_settings.setValue(PLAY_SOUNDS_ON_DISABLE_KEY, m_playSoundsOnDisable);
    m_settings.setValue(LANGUAGE_KEY, m_language);
    m_settings.setValue(MOUSE_SPEED_KEY, m_mouseSpeed);
    m_settings.setValue(MOUSE_ACCELERATION_KEY, m_mouseAcceleration);
    m_settings.setValue(MOUSE_SCROLL_SPEED_KEY, m_mouseScrollSpeed);
    m_settings.setValue(LEFT_JOYSTICK_DEADZONE_KEY, m_leftJoystickDeadZone);
    m_settings.setValue(RIGHT_JOYSTICK_DEADZONE_KEY, m_rightJoystickDeadZone);
}

void Settings::revert()
{
    setRunOnStartup(QFile::exists(RUN_ON_STARTUP_LNK_FILENAME));
    setPlaySoundsOnDisable(m_settings.value(PLAY_SOUNDS_ON_DISABLE_KEY, PLAY_SOUNDS_ON_DISABLE_DEFAULT).toBool());
    setLanguage(m_settings.value(LANGUAGE_KEY, LANGUAGE_STRING_DEFAULT).toString());
    setMouseSpeed(mouseSpeedNormalize(m_settings.value(MOUSE_SPEED_KEY, MOUSE_SPEED_DEFAULT).toDouble()));
    setMouseScrollSpeed(mouseScrollSpeedNormalize(m_settings.value(MOUSE_SCROLL_SPEED_KEY, MOUSE_SCROLL_SPEED_DEFAULT).toDouble()));
    setMouseAcceleration(mouseAccelerationNormalize(m_settings.value(MOUSE_ACCELERATION_KEY, MOUSE_ACCELERATION_DEFAULT).toDouble()));
    setLeftJoystickDeadZone(joystickNormalize(m_settings.value(LEFT_JOYSTICK_DEADZONE_KEY, JOYSTICK_DEADZONE_DEFAULT).toUInt()));
    setRightJoystickDeadZone(joystickNormalize(m_settings.value(RIGHT_JOYSTICK_DEADZONE_KEY, JOYSTICK_DEADZONE_DEFAULT).toUInt()));
}
