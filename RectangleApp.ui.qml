import QtQuick 2.12
import QtQuick.Controls 2.13
import com.tekit.powerpad.controllerthread 1.0

Rectangle {
    id: rectangle
    color: "#eeeeee"
    anchors.fill: parent
    border.width: 1
    width: 300
    height: 300

    Rectangle {
        id: rectangle1
        x: 1
        y: 1
        width: 298
        height: 70
        color: "#3f51b5"
    }

    Label {
        id: label
        x: 65
        y: 1
        height: 46
        text: qsTr("PowerPAD")
        font.bold: true
        font.pointSize: 10
        color: "white"
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        x: 66
        y: 35
        height: 25
        width: 69
        text: qsTr("Connected")
        font.italic: true
        font.pointSize: 10
        color: "white"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        visible: ControllerThread.connectedCount > 0
    }

    Image {
        id: image
        x: 15
        y: 14
        width: 39
        height: 39
        smooth: true
        source: "icon.png"
        fillMode: Image.PreserveAspectFit
    }

    Switch {
        id: enableSwitch
        x: 8
        y: 82
        checked: ControllerThread.enabled
        onCheckedChanged: ControllerThread.enabled = checked
        text: qsTr("Enabled")
        spacing: 16
    }
}
