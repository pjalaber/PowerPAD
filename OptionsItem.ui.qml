import QtQuick 2.4
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Page {
    id: element2
    width: 620
    height: 400
    property alias languageComboBox: languageComboBox
    property alias soundsOnDisableSwitch: soundsOnDisableSwitch
    property alias runOnStartupSwitch: runOnStartupSwitch
    property alias mouseSpeedSlider: mouseSpeedSlider
    property alias mouseAccelerationSlider: mouseAccelerationSlider
    property alias mouseScrollSpeedSlider: mouseScrollSpeedSlider
    property alias aboutLicenseTextArea: aboutLicenseTextArea
    property alias aboutText: aboutText
    property alias leftJoystickDeadZone: leftJoystickDeadZone
    property alias rightJoystickDeadZone: rightJoystickDeadZone

    header: TabBar {
        id: tabBar
        position: TabBar.Header
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

    contentData: StackLayout {
        id: stackLayout
        currentIndex: tabBar.currentIndex
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.bottomMargin: 20
        Item {
            Label {
                id: label
                text: qsTr("Language:")
                anchors.verticalCenter: languageComboBox.verticalCenter
                anchors.right: languageComboBox.left
                anchors.rightMargin: 5
            }
            ComboBox {
                id: languageComboBox
                anchors.right: parent.right
            }
            Switch {
                id: soundsOnDisableSwitch
                anchors.top: parent.top
                text: qsTr("Play sounds on disable")
            }
            Switch {
                id: runOnStartupSwitch
                anchors.top: soundsOnDisableSwitch.bottom
                anchors.topMargin: 10
                text: qsTr("Run on startup")
            }
        }

        Item {
            OptionsSlider {
                id: leftJoystickDeadZone
                lowText.text: qsTr("0%")
                labelText.text: qsTr("Left joystick dead zone:")
                highText.text: qsTr("100%")
                snapMode: "SnapAlways"
            }

            OptionsSlider {
                id: rightJoystickDeadZone
                lowText.text: qsTr("0%")
                labelText.text: qsTr("Right joystick dead zone:")
                highText.text: qsTr("100%")
                snapMode: "SnapAlways"
                anchors.top: leftJoystickDeadZone.bottom
                anchors.topMargin: 20
            }
        }

        Item {
            OptionsSlider {
                id: mouseSpeedSlider
                highText.text: qsTr("Fast")
                lowText.text: qsTr("Slow")
                labelText.text: qsTr("Mouse cursor speed:")
                snapMode: "SnapAlways"
            }

            OptionsSlider {
                id: mouseAccelerationSlider
                highText.text: qsTr("Fast")
                lowText.text: qsTr("Off")
                labelText.text: qsTr("Mouse cursor acceleration:")
                snapMode: "SnapAlways"
                anchors.top: mouseSpeedSlider.bottom
                anchors.topMargin: 20
            }

            OptionsSlider {
                id: mouseScrollSpeedSlider
                highText.text: qsTr("Fast")
                lowText.text: qsTr("Slow")
                labelText.text: qsTr("Scroll speed:")
                snapMode: "SnapAlways"
                anchors.top: mouseAccelerationSlider.bottom
                anchors.topMargin: 20
            }
        }

        Item {
            Rectangle {
                id: rectangle
                width: parent.width / 6
                height: parent.height
                color: "#174797"
                Image {
                    id: image
                    x: 8
                    y: 8
                    sourceSize.width: parent.width * 0.8
                    sourceSize.height: parent.width * 0.8
                    source: "images/icon.svg"
                    fillMode: Image.PreserveAspectFit
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                id: aboutText
                text: qsTr("About")
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignTop
                font.pixelSize: 12
                anchors.left: rectangle.right
                anchors.leftMargin: 10
            }

            ScrollView {
                id: scrollView
                anchors.left: rectangle.right
                anchors.top: aboutText.bottom
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.topMargin: 10
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                TextArea {
                    id: aboutLicenseTextArea
                    wrapMode: Text.WordWrap
                    placeholderText: qsTr("Text Area")
                }
            }
        }
    }

    footer: Item {
        height: parent.height * 0.10
        Button {
            id: okButton
            text: qsTr("OK")
            anchors.right: cancelButton.left
            anchors.rightMargin: 20
        }
        Button {
            id: cancelButton
            text: qsTr("Cancel")
            anchors.right: parent.right
            anchors.rightMargin: 20
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
