import QtQuick 2.4
import QtQuick.Controls 2.14

Item {
    width: 400
    height: 230
    property alias checkingUpdate: checkingUpdate
    property alias noUpdateAvailable: noUpdateAvailable
    property alias updateAvailable: updateAvailable
    property alias updateError: updateError
    property alias labelCurrentVersion: labelCurrentVersion
    property alias labelNewVersion: labelNewVersion

    Item {
        visible: false
        id: checkingUpdate
        anchors.fill: parent
        Label {
            id: labelChecking
            text: qsTr("Checking for updates...")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: separator1.top
        }

        Rectangle {
            id: separator1
            height: 5
            anchors.verticalCenter: parent.verticalCenter
        }

        BusyIndicator {
            id: busyIndicator
            wheelEnabled: false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: separator1.bottom
        }
    }

    Item {
        visible: false
        id: noUpdateAvailable
        anchors.fill: parent
        Label {
            id: labelNoUpdate
            text: qsTr("No update is available.")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: separator2.top
        }
        Rectangle {
            id: separator2
            height: 20
            anchors.verticalCenter: parent.verticalCenter
        }
        Button {
            id: noUpdateCloseButton
            text: qsTr("Close")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: separator2.bottom
        }
    }

    Item {
        visible: true
        id: updateAvailable
        anchors.fill: parent
        Label {
            id: labelUpdate
            text: qsTr("An updated version of PowerPAD is available.")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: labelCurrentVersion.top
            anchors.bottomMargin: 20
        }
        Label {
            id: labelCurrentVersion
            text: "Current version:"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: labelNewVersion.top
        }
        Label {
            id: labelNewVersion
            text: "New version:"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: separator3.top
        }
        Rectangle {
            id: separator3
            height: 20
            anchors.verticalCenter: parent.verticalCenter
        }
        Button {
            id: installButton
            text: qsTr("Install it now")
            anchors.right: parent.horizontalCenter
            anchors.top: separator3.bottom
            anchors.rightMargin: 5
        }
        Button {
            id: updateLaterButton
            text: qsTr("Later")
            anchors.left: parent.horizontalCenter
            anchors.top: separator3.bottom
            anchors.leftMargin: 5
        }
    }

    Item {
        visible: false
        id: updateError
        anchors.fill: parent
        Label {
            id: labelUpdateError
            text: qsTr("An error has occured while checking updates.")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: separator4.top
        }
        Rectangle {
            id: separator4
            height: 20
            anchors.verticalCenter: parent.verticalCenter
        }
        Button {
            id: errorCloseButton
            text: qsTr("Close")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: separator4.bottom
        }
    }

    signal accepted
    signal rejected

    Connections {
        target: installButton
        onClicked: accepted()
    }

    Connections {
        target: errorCloseButton
        onClicked: rejected()
    }

    Connections {
        target: noUpdateCloseButton
        onClicked: rejected()
    }

    Connections {
        target: updateLaterButton
        onClicked: rejected()
    }
}
