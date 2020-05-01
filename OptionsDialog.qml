import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.14
import com.tekit.powerpad.settings 1.0
import com.tekit.powerpad.helper 1.0

// options dialog
ApplicationWindow {
    title: qsTr("PowerPAD - options")
    flags: Qt.Dialog
    minimumWidth: optionsItem.width
    minimumHeight: optionsItem.height
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    property bool needRestart: false

    x: (screen.width - width) / 2
    y: (screen.height - height) / 2

    OptionsItem {
        id: optionsItem
        aboutText.text: Helper.aboutText
        runOnStartupSwitch {
            checked: Settings.runOnStartup
            onCheckedChanged: Settings.runOnStartup = runOnStartupSwitch.checked
        }
        soundsOnDisableSwitch {
            checked: Settings.playSoundsOnDisable
            onCheckedChanged: Settings.playSoundsOnDisable = soundsOnDisableSwitch.checked
        }
        languageComboBox {
            readonly property variant languages: ["SYSTEM", "en_EN", "fr_FR"]
            model: Array.from(languages, k => k == "SYSTEM" ? qsTr("Default system") : Qt.locale(k).nativeLanguageName)
            onActivated: {
                Settings.language = languages[index]
                optionsDialog.needRestart = true
            }
            currentIndex: languages.indexOf(Settings.language)
            delegate: ItemDelegate {
                width: optionsItem.languageComboBox.width
                contentItem: Text {
                    text: modelData
                    color: optionsItem.languageComboBox.highlightedIndex === index ? "white" :  "black"
                    font: optionsItem.languageComboBox.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
                highlighted: optionsItem.languageComboBox.highlightedIndex === index
            }
        }
        leftJoystickDeadZone {
            from: Settings.joystickDeadZoneMin()
            to: Settings.joystickDeadZoneMax()
            stepSize: (leftJoystickDeadZone.to - leftJoystickDeadZone.from) / 10
            value: Settings.leftJoystickDeadZone
            onValueChanged: Settings.leftJoystickDeadZone = leftJoystickDeadZone.value
            revertButton {
                onClicked: Settings.leftJoystickDeadZone = Settings.joystickDeadZoneDefault()
            }
        }
        rightJoystickDeadZone {
            from: Settings.joystickDeadZoneMin()
            to: Settings.joystickDeadZoneMax()
            stepSize: (rightJoystickDeadZone.to - rightJoystickDeadZone.from) / 10
            value: Settings.rightJoystickDeadZone
            onValueChanged: Settings.rightJoystickDeadZone = rightJoystickDeadZone.value
            revertButton {
                onClicked: Settings.rightJoystickDeadZone = Settings.joystickDeadZoneDefault()
            }
        }
        mouseSpeedSlider {
            from: Settings.mouseSpeedMin()
            to: Settings.mouseSpeedMax()
            stepSize: (mouseSpeedSlider.to - mouseSpeedSlider.from) / 10
            value: Settings.mouseSpeed
            onValueChanged: Settings.mouseSpeed = mouseSpeedSlider.value
            revertButton {
                onClicked: Settings.mouseSpeed = Settings.mouseSpeedDefault();
            }
        }

        onAccepted: optionsDialog.accept()
        onRejected: optionsDialog.reject()

        Component.onCompleted: {
            var request = new XMLHttpRequest()
            request.onreadystatechange = function() {
                if (request.readyState === XMLHttpRequest.DONE)
                    aboutLicenseTextArea.text = request.responseText
            }
            request.open("GET", "gplv3.txt")
            request.send()
        }
    }
    onActiveChanged: {
        if (active) { // when options dialog becomes active, hide application window
            needRestart = false
            applicationWindow.hideApp()
        }
    }
    onClosing: {
        close.accepted = false // refuse options dialog closing, just hide it
        reject()
    }
    function accept() {
        Settings.commit()
        hide()
        if (needRestart)
            Helper.restartApp()
    }
    function reject() {
        Settings.revert()
        hide()
    }
}
