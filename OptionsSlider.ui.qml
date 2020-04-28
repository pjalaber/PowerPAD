import QtQuick 2.4
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3

Slider {
    id: slider
    height: 50

    property alias labelText: labelText
    property alias lowText: lowText
    property alias highText: highText

    Text {
        id: labelText
        text: qsTr("Label")
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        font.pixelSize: 12
    }

    Text {
        id: lowText
        y: 38
        text: qsTr("Low")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        font.pixelSize: 10
    }

    Text {
        id: highText
        x: 179
        y: 38
        text: qsTr("High")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        font.pixelSize: 10
    }
}

/*##^##
Designer {
    D{i:1;anchors_x:0}D{i:3;anchors_x:0;anchors_y:0}
}
##^##*/

