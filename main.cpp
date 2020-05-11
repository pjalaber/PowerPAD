#include <QApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QIcon>
#include <QTranslator>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include "controller.h"
#include "helper.h"
#include "settings.h"
#include "keyboard.h"

class MyApplication: public QApplication
{
    using QApplication::QApplication;

    bool notify(QObject *object, QEvent *event)
    {
        qInfo().nospace() << QDateTime::currentDateTime().time().toString() << " " << object << ": " << event;
        return QApplication::notify(object, event);
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle("Fusion");

    MyApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/icon.png"));

    QTranslator translator;
    Settings *settings = Settings::instance();
    QString lang = settings->language();
    if (lang == Settings::LANGUAGE_STRING_SYSTEM)
        lang = QLocale::system().name();
    if (lang != "en_EN") {
        if (!translator.load(QString(":/languages/PowerPAD_%1.qm").arg(lang)))
            qDebug().nospace() << "Failed to load translator for language " << lang;
        else {
            if (!app.installTranslator(&translator))
                qDebug().nospace() << "Failed to install translator for language " << lang;
            else
                qDebug().nospace() << "Translator for language " << lang << " successfully installed";
        }
    }
    QLocale::setDefault(QLocale(lang));

    ControllerThread *controllerThread = ControllerThread::instance();
    controllerThread->start();

    qmlRegisterSingletonType<ControllerThread>("com.tekit.powerpad.controllerthread", 1, 0, "ControllerThread",
                                               [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return ControllerThread::instance();
    });

    qmlRegisterSingletonType<Helper>("com.tekit.powerpad.helper", 1, 0, "Helper",
                                               [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Helper::instance();
    });

    qmlRegisterSingletonType<Settings>("com.tekit.powerpad.settings", 1, 0, "Settings",
                                               [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Settings::instance();
    });

    qmlRegisterSingletonType<Keyboard>("com.tekit.powerpad.keyboard", 1, 0, "Keyboard",
                                               [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return Keyboard::instance();
    });

    QQuickWindow::setTextRenderType(QQuickWindow::TextRenderType::NativeTextRendering);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    int ret = app.exec();
    controllerThread->stop();
    return ret;
}
