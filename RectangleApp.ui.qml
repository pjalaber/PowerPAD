import QtQuick 2.12
import QtQuick.Controls 2.13

Rectangle {
    color: "#fafafa"
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
        color: "#3f51b5"
    }

    Label {
        x: 65
        y: 13
        width: 87
        height: 28
        text: qsTr("PowerPAD")
        font.bold: true
        font.pointSize: 12
        color: "white"
        verticalAlignment: Text.AlignVCenter
    }

    property alias labelStatus: labelStatus
    Label {
        id: labelStatus
        x: 65
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

    Image {
        x: 15
        y: 16
        width: 39
        height: 39
        layer.smooth: true
        smooth: true
        source: "icon.png"
        fillMode: Image.PreserveAspectFit
    }

    property alias switchEnable: switchEnable
    Switch {
        id: switchEnable
        x: 58
        y: 88
        width: 135
        height: 40
        text: qsTr("Enabled")
        font.bold: false
        font.pointSize: 11
        font.wordSpacing: 0
        wheelEnabled: false
        display: AbstractButton.TextBesideIcon
        spacing: 6
    }

    Label {
        x: 79
        y: 126
        width: 86
        height: 20
        text: qsTr("( Back + Start )")
        font.italic: true
        font.pointSize: 8
    }

    Rectangle {
        x: 1
        y: 167
        width: 248
        height: 62
        color: "#f0f0f0"
    }

    ToolButton {
        x: 37
        y: 167
        width: 86
        height: 61
        text: "Show<br>Options"
        checked: false
        checkable: false
        autoRepeat: false
        flat: false
        font.pointSize: 8
    }

    ToolButton {
        x: 123
        y: 167
        width: 86
        height: 61
        text: "Plus..."
        enabled: true
        font.pointSize: 8
    }
}
