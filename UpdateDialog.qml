import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import com.tekit.powerpad.update 1.0

// update dialog
ApplicationWindow {
    id: updateDialog
    title: qsTr("PowerPAD - update")
    flags: Qt.Dialog | Qt.MSWindowsFixedSizeDialogHint | Qt.WindowTitleHint | Qt.WindowSystemMenuHint | Qt.WindowCloseButtonHint
    width: updateItem.width
    height: updateItem.height
    x: (screen.width - width) / 2
    y: (screen.height - height) / 2

    UpdateItem {
        id: updateItem
        onAccepted: updateDialog.startUpdate()
        onRejected: updateDialog.hide()
    }

    onActiveChanged: {
        if (active) { // when update dialog becomes active, hide application window
            applicationWindow.hideApp()
        }
    }

    onClosing: {
        close.accepted = false // refuse update dialog closing, just hide it
        hide()
    }

    function setNothingVisible()
    {
        updateItem.checkingUpdate.visible = false
        updateItem.noUpdateAvailable.visible = false
        updateItem.updateAvailable.visible = false
        updateItem.updateError.visible = false
    }

    function setCheckingVisible()
    {
        updateItem.checkingUpdate.visible = true
        updateItem.noUpdateAvailable.visible = false
        updateItem.updateAvailable.visible = false
        updateItem.updateError.visible = false
    }

    function setNoUpdateVisible()
    {
        updateItem.checkingUpdate.visible = false
        updateItem.noUpdateAvailable.visible = true
        updateItem.updateAvailable.visible = false
        updateItem.updateError.visible = false
    }

    function setUpdateVisible(currentVersion, newVersion)
    {
        updateItem.labelCurrentVersion.text = qsTr("Current version: %1").arg(currentVersion)
        updateItem.labelNewVersion.text = qsTr("New version: %1").arg(newVersion)
        updateItem.checkingUpdate.visible = false
        updateItem.noUpdateAvailable.visible = false
        updateItem.updateAvailable.visible = true
        updateItem.updateError.visible = false
    }

    function setUpdateErrorVisible()
    {
        updateItem.checkingUpdate.visible = false
        updateItem.noUpdateAvailable.visible = false
        updateItem.updateAvailable.visible = false
        updateItem.updateError.visible = true
    }

    Connections {
        target: Update
        onCheckUpdateStarted: {
            setCheckingVisible()
        }
        onCheckUpdateFinishedNoUpdate: {
            setNoUpdateVisible()
        }
        onCheckUpdateFinishedUpdateAvailable: {
            setUpdateVisible(currentVersion, newVersion)
        }
        onCheckUpdateErrorOccured: {
            setUpdateErrorVisible()
        }
    }

    function startCheckUpdate()
    {
        setNothingVisible()
        Update.startCheckUpdate()
    }

    function startUpdate()
    {
        console.log("start update")
        hide()
    }
}
