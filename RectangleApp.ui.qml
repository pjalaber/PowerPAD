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
    antialiasing: false
    anchors.fill: parent
    border.width: 1
    width: 250
    height: 230

    Rectangle {
        x: 1
        y: 1
        width: 248
        height: 70
        color: "#174797"
    }

    Label {
        x: 74
        y: 10
        width: 87
        height: 28
        text: qsTr("PowerPAD")
        font.bold: true
        font.pointSize: 12
        color: "white"
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        id: labelStatus
        x: 74
        y: 35
        height: 25
        width: 69
        text: qsTr("Connected")
        font.italic: true
        font.pointSize: 10
        color: "white"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    property alias icon: icon
    Image {
        id: icon
        x: 14
        y: 12
        sourceSize.width: 44
        sourceSize.height: 44
        source: "images/icon.svg"
        fillMode: Image.PreserveAspectFit
    }

    Switch {
        id: switchEnable
        x: 58
        y: 94
        width: 154
        height: 40
        text: qsTr("Enabled")
        font.bold: false
        font.pointSize: 12
        font.wordSpacing: 0
        wheelEnabled: false
        display: AbstractButton.TextBesideIcon
        spacing: 15
    }

    Label {
        x: 58
        y: 132
        width: 86
        height: 20
        text: qsTr("( Back + Start )")
        font.italic: true
        font.pointSize: 10
    }

    ToolBar {
        id: toolBar
        x: 1
        y: 172
        width: 248
        height: 57

        ToolButton {
            id: moreButton
            x: 123
            y: 0
            width: 95
            height: 56
            text: qsTr("More...")
            autoRepeat: true
            font.pointSize: 9
        }

        ToolButton {
            id: optionsButton
            x: 27
            y: 0
            width: 95
            height: 56
            text: qsTr("Show<br>options")
            display: AbstractButton.TextBesideIcon
            font.pointSize: 9
        }
    }

    Text {
        id: statusErrorText
        y: 77
        height: 14
        color: "#e63333"
        text: "Status error"
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 14
        font.pixelSize: 12
    }
}

/*##^##
Designer {
    D{i:5;anchors_height:40;anchors_width:135;anchors_x:58;anchors_y:88}
}
##^##*/

