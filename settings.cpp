#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include "settings.h"

const QString Settings::PLAY_SOUNDS_ON_DISABLE_KEY = "General/PlaySoundsOnDisable";
const bool Settings::PLAY_SOUNDS_ON_DISABLE_DEFAULT = true;

const QString Settings::LANGUAGE_KEY = "General/Language";
const QString Settings::LANGUAGE_STRING_SYSTEM = "SYSTEM";
const QString Settings::LANGUAGE_STRING_DEFAULT = LANGUAGE_STRING_SYSTEM;

const QString Settings::MOUSE_SPEED_KEY = "Mouse/Speed";
const double Settings::MOUSE_SPEED_MIN = 1.0;
const double Settings::MOUSE_SPEED_DEFAULT = 5.0;
const double Settings::MOUSE_SPEED_MAX = 11.0;

const QString Settings::LEFT_JOYSTICK_DEADZONE_KEY = "Controller/LeftJoystickDeadZone";
const QString Settings::RIGHT_JOYSTICK_DEADZONE_KEY = "Controller/RightJoystickDeadZone";
const quint32 Settings::JOYSTICK_DEADZONE_MIN = 2000;
const quint32 Settings::JOYSTICK_DEADZONE_DEFAULT = 4000;
const quint32 Settings::JOYSTICK_DEADZONE_MAX = 12000;

double Settings::mouseSpeedNormalize(double mouseSpeed)
{
    return qMin(MOUSE_SPEED_MAX, qMax(MOUSE_SPEED_MIN, mouseSpeed));
}

quint32 Settings::joystickNormalize(quint32 joystickSpeed)
{
    return qMin(JOYSTICK_DEADZONE_MAX, qMax(JOYSTICK_DEADZONE_MIN, joystickSpeed));
}

Settings::Settings(QObject *parent) : QObject(parent),
    m_settings("Tekit", "PowerPAD"),
    m_winStartupSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat)
{
    revert();
}

Settings* Settings::instance()
{
    static Settings *s = new Settings();
    return s;
}

void Settings::setMouseSpeed(const double &mouseSpeed)
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

void Settings::setRunOnStartup(const bool &runOnStartup)
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

void Settings::setPlaySoundsOnDisable(const bool &playSoundsOnDisable)
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

void Settings::setLeftJoystickDeadZone(const quint32 &leftJoystickDeadZone)
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

void Settings::setRightJoystickDeadZone(const quint32 &rightJoystickDeadZone)
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
    if (m_runOnStartup)
         m_winStartupSettings.setValue(APP_PRODUCT, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    else
         m_winStartupSettings.remove(APP_PRODUCT);

    m_settings.setValue(PLAY_SOUNDS_ON_DISABLE_KEY, m_playSoundsOnDisable);
    m_settings.setValue(LANGUAGE_KEY, m_language);
    m_settings.setValue(MOUSE_SPEED_KEY, m_mouseSpeed);
    m_settings.setValue(LEFT_JOYSTICK_DEADZONE_KEY, m_leftJoystickDeadZone);
    m_settings.setValue(RIGHT_JOYSTICK_DEADZONE_KEY, m_rightJoystickDeadZone);
}

void Settings::revert()
{
    setRunOnStartup(!m_winStartupSettings.value(APP_PRODUCT, "").toString().isEmpty());
    setPlaySoundsOnDisable(m_settings.value(PLAY_SOUNDS_ON_DISABLE_KEY, PLAY_SOUNDS_ON_DISABLE_DEFAULT).toBool());
    setLanguage(m_settings.value(LANGUAGE_KEY, LANGUAGE_STRING_DEFAULT).toString());
    setMouseSpeed(mouseSpeedNormalize(m_settings.value(MOUSE_SPEED_KEY, MOUSE_SPEED_DEFAULT).toDouble()));
    setLeftJoystickDeadZone(joystickNormalize(m_settings.value(LEFT_JOYSTICK_DEADZONE_KEY, JOYSTICK_DEADZONE_DEFAULT).toUInt()));
    setRightJoystickDeadZone(joystickNormalize(m_settings.value(RIGHT_JOYSTICK_DEADZONE_KEY, JOYSTICK_DEADZONE_DEFAULT).toUInt()));
}
