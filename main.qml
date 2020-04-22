import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.14
import Qt.labs.platform 1.1
import com.tekit.powerpad.controllerthread 1.0
import com.tekit.powerpad.helper 1.0

ApplicationWindow {
    id: applicationWindow
    flags: Qt.FramelessWindowHint
    visible: false
    width: 250
    height: 230

    // application window rectangle
    RectangleApp {
        id: rectangleApp

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
                    qsTr("")
            }
        }

        // on/off switch that enables/disables the controller
        switchEnable {
            checked: ControllerThread.enabled
            onCheckedChanged: ControllerThread.enabled = switchEnable.checked
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
    ApplicationWindow {
        title: qsTr("PowerPAD options")
        flags: Qt.Dialog
        id: optionsDialog
        x: (screen.width - width) / 2
        y: (screen.height - height) / 2
        onActiveChanged: {
            if (active) // when options dialog becomes active, hide application window
                applicationWindow.hideApp()
        }
        onClosing: {
            close.accepted = false // refuse options dialog closing, just hide it
            hide()
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
