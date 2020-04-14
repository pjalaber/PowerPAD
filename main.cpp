#include <QApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>
#include "controller.h"

int main(int argc, char *argv[])
{
    ControllerThread *controllerThread = &ControllerThread::instance();
    if (!controllerThread->start())
        return -1;

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle("Universal");

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icon.png"));

    qmlRegisterSingletonType<ControllerThread>("com.tekit.powerpad.controllerthread", 1, 0, "ControllerThread",
                                               [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return &ControllerThread::instance();
    });


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
