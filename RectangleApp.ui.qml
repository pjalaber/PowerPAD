import QtQuick 2.14
import QtQuick.Controls 2.14

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
    width: 250
    height: 230

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
                x: parent.x + parent.width + 5
                y: icon.y + icon.height / 4
                text: qsTr("PowerPAD")
                anchors.topMargin: 0
                font.bold: true
                font.pointSize: 12
                color: "white"
            }

            Label {
                id: labelStatus
                x: labelTitle.x
                y: labelTitle.y + labelTitle.height + 3
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
        y: topRectangle.y + topRectangle.height + 10
        color: "#e63333"
        text: "Status error"
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 12
    }

    Switch {
        id: switchEnable
        y: statusErrorText.y + statusErrorText.height + 5
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
        y: switchEnable.y + switchEnable.height
        text: qsTr("( Back + Start )")
        anchors.horizontalCenter: parent.horizontalCenter
        font.italic: true
        font.pointSize: 10
    }

    ToolBar {
        id: toolBar
        x: 1
        y: parent.height * 3.0 / 4
        height: parent.height / 4 - 1
        width: parent.width - 2
        ToolButton {
            id: optionsButton
            x: parent.width * 2.5 / 25
            width: parent.width * 10.0 / 25
            height: parent.height
            text: qsTr("Show<br>options")
            display: AbstractButton.TextBesideIcon
            font.pointSize: 9
        }

        ToolButton {
            id: moreButton
            x: optionsButton.x + optionsButton.width
            width: optionsButton.width
            height: parent.height
            text: qsTr("More...")
            autoRepeat: true
            font.pointSize: 9
        }
    }
}
