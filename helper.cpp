#include <QDebug>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QProcess>
#include <QScreen>
#include "helper.h"

Helper::Helper(QObject *parent) : QObject(parent)
{
}

Helper* Helper::instance()
{
    static Helper *h = new Helper();
    return h;
}

void Helper::restartApp()
{
    QCoreApplication *app = QCoreApplication::instance();
    app->quit();
    QProcess::startDetached(app->arguments()[0], app->arguments());
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

    QRect desktop = s->availableGeometry();
    QRect screen = s->geometry();
    QPoint p;

    if (desktop.y() + desktop.height() < screen.y() + screen.height()) { /* taskbar is at the botoom */
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

QString Helper::aboutText()
{
    return "<b>" APP_PRODUCT " " APP_VERSION "</b><br><br>"
            "Built on " APP_BUILD_DATE "<br><br>"
            "From revision " APP_BUILD_REVISION "<br><br>"
            "Copyright " APP_COPYRIGHT;
}
