import QtQuick 2.4
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3

Item {
    width: 620
    height: 400
    property alias soundsOnDisableSwitch: soundsOnDisableSwitch
    property alias runOnStartupSwitch: runOnStartupSwitch
    property alias mouseSpeedSlider: mouseSpeedSlider
    property alias aboutLicenseTextArea: aboutLicenseTextArea

    property alias aboutText: aboutText

    TabBar {
        id: tabBar
        position: TabBar.Header
        anchors.bottomMargin: 56
        anchors.fill: parent
        currentIndex: 0

        TabButton {
            text: qsTr("General")
        }

        TabButton {
            text: qsTr("Controller")
        }

        TabButton {
            text: qsTr("Mouse")
        }

        TabButton {
            text: qsTr("About")
        }
    }

    StackLayout {
        id: stackLayout
        x: 20
        y: 40
        width: 580
        height: 304
        currentIndex: tabBar.currentIndex

        Item {
            Switch {
                id: soundsOnDisableSwitch
                x: 0
                y: 46
                text: qsTr("Play sounds on disable")
            }

            Switch {
                id: runOnStartupSwitch
                text: qsTr("Run on startup")
            }
        }

        Item {}

        Item {
            OptionsSlider {
                id: mouseSpeedSlider
                x: 0
                y: 0
                width: 319
                height: 50
                highText.text: qsTr("Fast")
                lowText.text: qsTr("Slow")
                labelText.text: qsTr("Mouse cursor speed:")
                snapMode: "SnapAlways"
                from: 1
                to: 10
                stepSize: 1
            }
        }

        Item {
            id: element
            Text {
                id: aboutText
                x: 152
                y: 8
                width: 428
                height: 116
                text: qsTr("About")
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignTop
                font.pixelSize: 12
            }

            ScrollView {
                id: scrollView
                x: 152
                y: 130
                width: 428
                height: 175
                anchors.bottom: parent.bottom
                anchors.bottomMargin: -1
                anchors.right: parent.right
                anchors.rightMargin: 0
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                TextArea {
                    id: aboutLicenseTextArea
                    x: -10
                    y: -6
                    width: 428
                    height: 175
                    wrapMode: Text.WordWrap
                    placeholderText: qsTr("Text Area")
                }
            }

            Rectangle {
                id: rectangle
                width: 120
                height: 296
                color: "#174797"
                border.color: "#6498ed"
                border.width: 0

                Image {
                    id: image
                    x: 8
                    y: 8
                    sourceSize.width: 100
                    sourceSize.height: 100
                    source: "images/icon.png"
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }

    Flow {
        id: element1
        anchors.rightMargin: 20
        spacing: 20
        flow: Flow.LeftToRight
        layoutDirection: Qt.RightToLeft
        anchors.topMargin: 361
        anchors.fill: parent

        Button {
            id: cancelButton
            text: qsTr("Cancel")
        }

        Button {
            id: okButton
            text: qsTr("OK")
            flat: false
        }
    }

    signal accepted
    Connections {
        target: okButton
        onClicked: accepted()
    }

    signal rejected
    Connections {
        target: cancelButton
        onClicked: rejected()
    }
}

/*##^##
Designer {
    D{i:2;anchors_width:240}D{i:1;anchors_height:200;anchors_width:200}D{i:16;anchors_x:"-5";anchors_y:0}
}
##^##*/

