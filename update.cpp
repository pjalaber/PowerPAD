#include <QCoreApplication>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>
#include <QDomDocument>
#include <QDebug>
#include "update.h"

Update::Update(QObject *parent) : QObject(parent), m_checkUpdateProcess(this), m_availableVersion("")
{
    const QString toolExe = "/PowerPADMaintenanceTool.exe";
    QString maintenanceToolExe = QCoreApplication::applicationDirPath() + "/" + toolExe;
    if (!QFile::exists(maintenanceToolExe))
        maintenanceToolExe = "c:/program files/PowerPAD/" + toolExe;

    m_checkUpdateProcess.setProgram(maintenanceToolExe);
    m_checkUpdateProcess.setArguments(QStringList({"--checkupdates", "NoLogIfNoInstall=true"}));
    m_checkUpdateProcess.setProcessChannelMode(QProcess::MergedChannels);

    connect(&m_checkUpdateProcess, &QProcess::started,
            [=]() {
        emit checkUpdateStarted();
    });

    connect(&m_checkUpdateProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=](int exitCode, QProcess::ExitStatus exitStatus) {
        bool updateAvailable = (exitCode == 0 && exitStatus == QProcess::NormalExit);
        if (!updateAvailable) {
            emit checkUpdateFinishedNoUpdate();
            return;
        }

        QDomDocument xml;
        xml.setContent(m_checkUpdateProcess.readAll());

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

    connect(&m_checkUpdateProcess, QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred),
            [=](QProcess::ProcessError error) {
        emit checkUpdateErrorOccured(error);
    });

    m_updateProcess.setProgram(maintenanceToolExe);
    m_updateProcess.setArguments(QStringList{"--updater", "NoLogIfNoInstall=true"});
    m_updateProcess.setProcessChannelMode(QProcess::MergedChannels);
}

Update* Update::instance()
{
    static Update *u = new Update();
    return u;
}

void Update::startCheckUpdate(void)
{
    if (m_checkUpdateProcess.state() == QProcess::ProcessState::NotRunning) {
        m_availableVersion = "";
        m_checkUpdateProcess.start();
    }
}

void Update::startUpdate(void)
{
    if (m_updateProcess.state() == QProcess::ProcessState::NotRunning)
        m_updateProcess.start();
}
