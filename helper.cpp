#include <QDebug>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QProcess>
#include <QScreen>
#include <QDateTime>
#include <QDir>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include "helper.h"

Helper::Helper(QObject *parent) : QObject(parent),
      m_winSystemSoundSettings("HKEY_CURRENT_USER\\AppEvents\\Schemes\\Apps\\.Default", QSettings::NativeFormat),
      m_foregroundWindow(nullptr)
{
    QList<QScreen *> l = QGuiApplication::screens();
    for (QList<QScreen *>::const_iterator i = l.constBegin(); i != l.constEnd(); i++)
    {
        QScreen *s = *i;
        qInfo() << s->name();
        qInfo() << connect(s, &QScreen::logicalDotsPerInchChanged, this, &Helper::logicalDotsPerInchChanged);
    }
}

Helper* Helper::instance()
{
    static Helper *h = new Helper();
    return h;
}

void Helper::logicalDotsPerInchChanged(qreal dpi)
{
    Q_UNUSED(dpi);
    qInfo() << "logical: " << dpi;

}

void Helper::restartApp()
{
    QCoreApplication *app = QCoreApplication::instance();
    app->quit();
    QProcess::startDetached(app->arguments()[0], app->arguments());
}

bool Helper::isAlreadyRunning()
{
    QSystemSemaphore semaphore("PowerPAD_sem", 1);
    semaphore.acquire();

    QSharedMemory sharedMemory("PowerPAD_shm");
    bool isRunning = sharedMemory.attach();
    if (!isRunning)
        sharedMemory.create(1);
    semaphore.release();

    return isRunning;
}


QRect Helper::computeBestWindowRect(const QString &screenName, const QRect &r, qint32 windowWidth, qint32 windowHeight)
{
    QList<QScreen *> l = QGuiApplication::screens();
    QScreen *s = nullptr;
    for (QList<QScreen *>::const_iterator i = l.constBegin(); i != l.constEnd(); i++)
    {
        s = *i;
        if ( s->name() == screenName) {
            break;
        }
    }

    if (s == nullptr)
        return r;

    QRect desktop = s->availableGeometry();
    QRect screen = s->geometry();
    QPoint p;

    if (desktop.y() + desktop.height() < screen.y() + screen.height()) { /* taskbar is at the bottom */
        /* try to place new rect horizontally centered above given rect */
        p = QPoint(r.x() + r.width() / 2 - windowWidth / 2, r.y() - windowHeight);
    } else if (desktop.y() > screen.y()) { /* taskbar is on top */
        /* try to place new rect horizontally centered under given rect */
        p = QPoint(r.x() + r.width() / 2 - windowWidth / 2, r.y() + r.height());
    } else if (desktop.x() > screen.x()) { /* taskbar is on left */
        /* try to place new rect vertically centered on right of given rect */
        p = QPoint(r.x() + r.width(), r.y() + r.height() / 2 - windowHeight / 2);
    } else if (desktop.x() + desktop.width() < screen.x() + screen.width()) { /* taskbar is on right */
        /* try to place new rect vertically centered on left of given rect */
        p = QPoint(r.x() - r.width(), r.y() + r.height() / 2 - windowHeight / 2);
    }
    if (p.rx() + windowWidth > desktop.x() + desktop.width())
        p.rx() = desktop.x() + desktop.width() - windowWidth;
    if (p.ry() + windowHeight > desktop.y() + desktop.height())
        p.ry() = desktop.y() + desktop.height() - windowHeight;
    if (p.rx() < desktop.x())
        p.rx() = desktop.x();
    if (p.ry() < desktop.y())
        p.ry() = desktop.y();

    return QRect(p.rx(), p.ry(), windowWidth, windowHeight);
}

QString Helper::getAboutText()
{
    QDateTime dt;
    dt.setSecsSinceEpoch(QString(APP_BUILD_DATE).toUInt());
    QLocale locale;
    return QString("<b>%1 %2</b><br><br>%3<br><br>%4<br><br>%5")
            .arg(APP_PRODUCT)
            .arg(APP_VERSION)
            .arg(tr("Built on %1").arg(locale.toString(dt, locale.dateTimeFormat(QLocale::FormatType::ShortFormat))))
            .arg(tr("From revision %1").arg(APP_BUILD_REVISION))
            .arg(tr("Copyright %1").arg(APP_COPYRIGHT));
}

QString Helper::getDeviceConnectSoundFilename()
{
    return QString("file:///%1").arg(QDir::fromNativeSeparators(m_winSystemSoundSettings.value("DeviceConnect/.Current/.", "").toString()));
}


QString Helper::getDeviceDisconnectSoundFilename()
{
    return QString("file:///%1").arg(QDir::fromNativeSeparators(m_winSystemSoundSettings.value("DeviceDisconnect/.Current/.", "").toString()));
}

void Helper::saveForegroundWindow()
{
    m_foregroundWindow = GetForegroundWindow();
}

void Helper::restoreForegroundWindow()
{
    if (m_foregroundWindow != nullptr)
        SetForegroundWindow(m_foregroundWindow);
}

