import QtQuick 2.12
import Qt.labs.platform 1.1
import com.tekit.powerpad.controllerthread 1.0

SystemTrayIcon
{
    tooltip: qsTr("PowerPAD")
    visible: true
    icon.mask: true    
    icon.source: {
        if (!ControllerThread.enabled)
            "icon_disabled.png"
        else
            "icon.png"
    }

    menu: Menu {
        MenuItem {
            text: qsTr("Show")
            onTriggered: applicationWindow.showApp(geometry)
        }
        MenuItem {
            text: qsTr("Quit")
            onTriggered: Qt.quit()
        }
    }

    onActivated: {
        if (!applicationWindow.visibility && reason !== SystemTrayIcon.Context)
        {
            applicationWindow.showApp(geometry)
        }
    }
}
