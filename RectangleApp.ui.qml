import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

Rectangle {
    id: rectangle
    color: "#fafafa"
    property alias optionsButton: optionsButton
    property alias labelStatus: labelStatus
    property alias switchEnable: switchEnable
    property alias moreButton: moreButton
    property alias statusErrorText: statusErrorText
    anchors.fill: parent
    border.width: 1
    width: 230
    height: 200

    Rectangle {
        id: topRectangle
        x: 1
        y: 1
        width: parent.width - 2
        height: parent.height / 3.5
        color: "#174797"

        property alias icon: icon
        Image {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            sourceSize.width: topRectangle.width / 6
            sourceSize.height: topRectangle.width / 6
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            source: "images/icon.svg"
            fillMode: Image.PreserveAspectFit

            Label {
                id: labelTitle
                y: icon.y + icon.height / 5
                anchors.left: icon.right
                anchors.leftMargin: 10
                text: qsTr("PowerPAD")
                font.bold: true
                font.pointSize: 12
                color: "white"
            }

            Label {
                id: labelStatus
                anchors.left: icon.right
                anchors.leftMargin: 10
                anchors.top: labelTitle.bottom
                anchors.topMargin: 5
                text: qsTr("Connected")
                font.italic: true
                font.pointSize: 10
                color: "white"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    Text {
        id: statusErrorText
        anchors.top: topRectangle.bottom
        anchors.topMargin: 5
        color: "#e63333"
        text: "Status error"
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 12
    }

    Switch {
        id: switchEnable
        anchors.top: statusErrorText.bottom
        anchors.topMargin: 5
        text: qsTr("Enabled")
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: false
        font.pointSize: 12
        font.wordSpacing: 0
        wheelEnabled: false
        display: AbstractButton.TextBesideIcon
        spacing: 15
    }

    Label {
        anchors.top: switchEnable.bottom
        anchors.topMargin: 0
        text: qsTr("( Back + Start )")
        anchors.horizontalCenter: parent.horizontalCenter
        font.italic: true
        font.pointSize: 10
    }

    ToolBar {
        id: toolBar
        x: 1
        height: parent.height / 4
        anchors.bottom: parent.bottom
        width: parent.width - 2
        RowLayout {
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            ToolButton {
                id: optionsButton
                height: parent.height
                text: qsTr("Show<br>options")
                Layout.fillHeight: true
                display: AbstractButton.TextBesideIcon
                font.pointSize: 9
            }

            ToolButton {
                id: moreButton
                height: parent.height
                text: qsTr("More...")
                Layout.fillHeight: true
                font.pointSize: 9
            }
        }
    }
}
