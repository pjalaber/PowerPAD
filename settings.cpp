#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include "settings.h"

const QString Settings::MOUSE_SPEED_STRING = "mouse/speed";
const double Settings::MOUSE_SPEED_MIN = 1.0;
const double Settings::MOUSE_SPEED_DEFAULT = 6.0;
const double Settings::MOUSE_SPEED_MAX = 10.0;

const QString Settings::RUN_ON_STARTUP_STRING = "general/runonstartup";
const bool Settings::RUN_ON_STARTUP_DEFAULT = true;

double Settings::mouseSpeedNormalize(double mouseSpeed)
{
    return qMin(MOUSE_SPEED_MAX, qMax(MOUSE_SPEED_MIN, mouseSpeed));
}

Settings::Settings(QObject *parent) : QObject(parent), m_settings("Tekit", "PowerPAD")
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

void Settings::setRunOnStartup(bool runOnStartup)
{
    if (m_runOnStartup == runOnStartup)
        return;
    m_runOnStartup = runOnStartup;
    emit runOnStartupChanged();

    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (m_runOnStartup)
        settings.setValue(APP_PRODUCT, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    else
        settings.remove(APP_PRODUCT);
    settings.sync();
}

bool Settings::runOnStartup()
{
    return m_runOnStartup;
}

void Settings::commit()
{
    m_settings.setValue(MOUSE_SPEED_STRING, m_mouseSpeed);
    m_settings.setValue(RUN_ON_STARTUP_STRING, m_runOnStartup);
}

void Settings::revert()
{
    setMouseSpeed(mouseSpeedNormalize(m_settings.value(MOUSE_SPEED_STRING, MOUSE_SPEED_DEFAULT).toDouble()));
    setRunOnStartup(m_settings.value(RUN_ON_STARTUP_STRING, RUN_ON_STARTUP_DEFAULT).toBool());
}
