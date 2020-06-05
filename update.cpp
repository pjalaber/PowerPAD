#include <QCoreApplication>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>
#include <QDomDocument>
#include <QDebug>
#include "update.h"

Update::Update(QObject *parent) : QObject(parent), m_process(this), m_availableVersion("")
{
    QString maintenanceToolExe = QCoreApplication::applicationDirPath() + "/maintenancetool.exe";
    if (!QFile::exists(maintenanceToolExe))
        maintenanceToolExe = "C:/Program Files/PowerPAD/maintenancetool.exe";

    m_process.setProgram(maintenanceToolExe);
    m_process.setArguments(QStringList("--checkupdates"));
    m_process.setProcessChannelMode(QProcess::MergedChannels);

    connect(&m_process, &QProcess::started,
            [=]() {
        emit checkUpdateStarted();
    });

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
        bool updateAvailable = (exitCode == 0 && exitStatus == QProcess::NormalExit);
        if (!updateAvailable) {
            emit checkUpdateFinishedNoUpdate();
            return;
        }

        QDomDocument xml;
        xml.setContent(m_process.readAll());

        QDomElement root = xml.documentElement();
        if (root.tagName() != "updates") {
            emit checkUpdateFinishedNoUpdate();
            return;
        }

        QDomElement update = root.firstChild().toElement();
        QString version = update.attribute("version");
        if (version == "") {
            emit checkUpdateFinishedNoUpdate();
            return;
        }
        m_availableVersion = version;
        emit checkUpdateFinishedUpdateAvailable(APP_VERSION, m_availableVersion);
    });

    connect(&m_process, QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred),
            [=](QProcess::ProcessError error) {
        emit checkUpdateErrorOccured(error);
    });
}

Update* Update::instance()
{
    static Update *u = new Update();
    return u;
}

void Update::startCheckUpdate(void)
{
    if (m_process.state() == QProcess::ProcessState::NotRunning) {
        m_availableVersion = "";
        m_process.start();
    }
}
