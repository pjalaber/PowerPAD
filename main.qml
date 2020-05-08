import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.14
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1
import QtLocation 5.14
import Qt.labs.folderlistmodel 2.12
import QtMultimedia 5.12
import com.tekit.powerpad.controllerthread 1.0
import com.tekit.powerpad.settings 1.0
import com.tekit.powerpad.helper 1.0
import com.tekit.powerpad.keyboard 1.0

ApplicationWindow {
    id: applicationWindow
    flags: Qt.FramelessWindowHint
    visible: false
    width: 250
    height: 230

    // display a popup message on desktop when a controller is connected
    // or disconnected
    Connections {
        property int oldConnectedCount: 0
        target: ControllerThread
        onConnectedCountChanged: {
            if (ControllerThread.connectedCount > oldConnectedCount)
                sysTray.showMessage("PowerPAD", qsTr("Controller has been connected"))
            else
                sysTray.showMessage("PowerPAD", qsTr("Controller has been disconnected"))
            oldConnectedCount = ControllerThread.connectedCount
        }
        onEnabledChanged: {
            if (ControllerThread.enabled)
                sysTray.showMessage("PowerPAD", qsTr("PowerPAD has been enabled"))
            else
                sysTray.showMessage("PowerPAD", qsTr("PowerPAD has been disabled"))
        }
        onShowKeyboardChanged: {
            if (ControllerThread.showKeyboard)
                keyboardItem.visible = true
            else
                keyboardItem.visible = false
        }
    }

    // application window rectangle
    RectangleApp {
        id: rectangleApp
        enabled: ControllerThread.status == ControllerThread.StatusOK

        // application window label status that shows:
        // - "Connected" if a controller is connected
        // - "Disabled" if user has disabled controller with Back+Start or with on/off switch
        // - "" if no controller is connected
        labelStatus {
            text: {
                if (!ControllerThread.enabled)
                    qsTr("Disabled")
                else if (ControllerThread.connectedCount > 0)
                    qsTr("Connected")
                else
                    ""
            }
        }

        statusErrorText {
            visible: ControllerThread.status != ControllerThread.StatusOK
            text: {
               if (ControllerThread.status == ControllerThread.StatusXInputLibraryNotFound)
                    qsTr("Error: xinput library not found !");
               else if (ControllerThread.status == ControllerThread.StatusXInputSymbolNotFound)
                   qsTr("Error: xinput symbol not found !");
               else if (ControllerThread.status == ControllerThread.StatusShell32NotFound)
                   qsTr("Error: shell32 library not found !");
               else if (ControllerThread.status == ControllerThread.StatusShell32SymbolNotFound)
                   qsTr("Error: shell32 symbol not found !");
               else
                   ""
            }
        }

        SoundEffect {
            id: deviceConnectSound
            source: Helper.getDeviceConnectSoundFilename()
        }

        SoundEffect {
            id: deviceDisconnectSound
            source: Helper.getDeviceDisconnectSoundFilename()
        }

        // on/off switch that enables/disables the controller
        switchEnable {
            checked: ControllerThread.enabled
            onCheckedChanged: {
                ControllerThread.enabled = switchEnable.checked
                if (Settings.playSoundsOnDisable) {
                    if (ControllerThread.enabled)
                        deviceConnectSound.play()
                    else
                        deviceDisconnectSound.play()
                }
            }
        }

        // more button that shows context menu when triggered
        moreButton {
            action: moreAction
        }

        // options button that show options dialog when triggered
        optionsButton {
            action: optionsAction
        }

        // more action associated to more button
        Action {
            id: moreAction
            icon.source: "images/more.png"
            onTriggered: {
                contextMenu.open(rectangleApp.moreButton, contextMenu.exitMenuItem)
            }
        }

        // options action associated to options button
        Action {
            id: optionsAction
            icon.source: "images/options.png"
            onTriggered: {
                if (!optionsDialog.visible)
                    applicationWindow.close()
                optionsDialog.show()
                optionsDialog.raise()
                optionsDialog.requestActivate()
            }
        }

        ContextMenu {
            id: contextMenu
        }
    }

    // options dialog
    OptionsDialog {
        id: optionsDialog
    }

    Window {
        id: keyboardWindow
        flags: Qt.Tool | Qt.FramelessWindowHint| Qt.WindowStaysOnTopHint | Qt.WindowTransparentForInput
        minimumWidth: keyboardItem.width
        minimumHeight: keyboardItem.height
        maximumWidth: minimumWidth
        maximumHeight: minimumHeight
        x: (screen.width - width) / 2
        y: (screen.height - height) / 2
        color: "transparent"
        visible: true
        KeyboardItem {
            id: keyboardItem
            visible: false
            character0.text: Keyboard.getCharacterAt(Keyboard.characterIndex - 5)
            character1.text: Keyboard.getCharacterAt(Keyboard.characterIndex - 4)
            character2.text: Keyboard.getCharacterAt(Keyboard.characterIndex - 3)
            character3.text: Keyboard.getCharacterAt(Keyboard.characterIndex - 2)
            character4.text: Keyboard.getCharacterAt(Keyboard.characterIndex - 1)
            character5.text: Keyboard.getCharacterAt(Keyboard.characterIndex)
            character6.text: Keyboard.getCharacterAt(Keyboard.characterIndex + 1)
            character7.text: Keyboard.getCharacterAt(Keyboard.characterIndex + 2)
            character8.text: Keyboard.getCharacterAt(Keyboard.characterIndex + 3)
            character9.text: Keyboard.getCharacterAt(Keyboard.characterIndex + 4)
            character10.text: Keyboard.getCharacterAt(Keyboard.characterIndex + 5)
        }

        Component.onCompleted: {
            keyboardWindow.show()
        }
    }

    SysTray {
        id: sysTray
    }

    // timer that prevents to execute during a short time systray onActivated callback
    Timer {
        id: timerReenterSysTray
        interval: 400
        repeat: false
        running: false
    }

    // opacity animation when showing application window
    NumberAnimation {
        id: opacityAnimation
        target: applicationWindow
        property: "opacity"
        from: 0;
        to: 1;
        duration: 200
        running: false
    }

    function showApp(nextToRect) {
        showNextToRect(nextToRect)
    }

    function hideApp() {
        // start a timer that prevents to execute during a short time systray onActivated callback,
        // otherwise it would lead to hide the application window, and just show it after.
        timerReenterSysTray.start();
        hide();
    }

    // show window application next to given rect, with an opacity animation
    function showNextToRect(nextToRect)
    {
        var dr = Helper.computeBestWindowRect(screen.name, nextToRect, width, height);
        x = dr.x
        y = dr.y
        opacity = opacityAnimation.from
        show()
        raise()
        requestActivate()
        opacityAnimation.start()
    }

    // called when window application is activated/desactivated
    onActiveChanged: {
        if (!active) // if desactivated hide window application
            hideApp();
    }

    // update context menu items depending on application window is visible or not
    onVisibleChanged: {
        contextMenu.showMenuItem.visible = contextMenu.separatorMenuItem.visible = !visible
    }
}
