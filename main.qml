import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtMultimedia 5.14
import com.tekit.powerpad.winsys 1.0
import com.tekit.powerpad.controllerthread 1.0
import com.tekit.powerpad.settings 1.0
import com.tekit.powerpad.helper 1.0

ApplicationWindow {
    id: applicationWindow
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    visible: false
    width: 230
    height: 200
    maximumWidth: width
    maximumHeight: height

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
    }

    // application window rectangle
    RectangleApp {
        id: rectangleApp
        enabled: ControllerThread.status === ControllerThread.StatusOK

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
            visible: WinSys.status != WinSys.StatusOK
            text: {
               if (WinSys.status == WinSys.StatusXInputLibraryNotFound)
                    qsTr("Error: xinput library not found !");
               else if (WinSys.status == WinSys.StatusXInputSymbolNotFound)
                   qsTr("Error: xinput symbol not found !");
               else if (WinSys.status == WinSys.StatusShell32NotFound)
                   qsTr("Error: shell32 library not found !");
               else if (WinSys.status == WinSys.StatusShell32SymbolNotFound)
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
            icon.source: "images/more.svg"
            onTriggered: {
                contextMenu.open(rectangleApp.moreButton, contextMenu.exitMenuItem)
            }
        }

        // options action associated to options button
        Action {
            id: optionsAction
            icon.source: "images/options.svg"
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

    KeyboardWindow {
        id: keyboardWindow
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
