import QtQuick 2.12
import QtQuick.Controls 2.13
import com.tekit.powerpad.controllerthread 1.0

Rectangle {
    color: "#ffffff"
    anchors.fill: parent
    border.width: 1
    width: 300
    height: 300

    Rectangle {
        id: rectangle1
        x: 1
        y: 1
        width: 300
        height: 46
        color: "#000000"
    }

    Label {
        id: label
        x: 60
        y: 1
        width: 240
        height: 46
        text: qsTr("PowerPAD")
        font.bold: true
        font.pointSize: 10
        color: "white"
        verticalAlignment: Text.AlignVCenter
    }

    Image {
        id: image
        x: 15
        y: 4
        width: 32
        height: 42
        source: "icon.png"
        fillMode: Image.PreserveAspectFit
    }

    Switch {
        id: enableSwitch
        x: 8
        y: 50
        checked: ControllerThread.enabled
        onCheckedChanged: ControllerThread.enabled = checked
        text: qsTr("Enabled")
    }
}



