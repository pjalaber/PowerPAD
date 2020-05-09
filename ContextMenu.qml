import QtQuick 2.14
import Qt.labs.platform 1.1
import com.tekit.powerpad.helper 1.0

Menu {
    property alias showMenuItem: showMenuItem
    property alias exitMenuItem: exitMenuItem
    property alias separatorMenuItem: separatorMenuItem

    MenuItem {
        id: showMenuItem
        text: qsTr("Show")
        onTriggered: applicationWindow.showApp(sysTray.geometry)
    }

    MenuSeparator {
        id: separatorMenuItem
    }

    MenuItem {
        text: qsTr("Check for updates")
    }

    MenuSeparator{
    }

    MenuItem {
        text: qsTr("Restart")
        onTriggered: Helper.restartApp()
    }

    MenuItem {
        id: exitMenuItem
        text: qsTr("Exit")
        onTriggered: Qt.quit()
    }
}
