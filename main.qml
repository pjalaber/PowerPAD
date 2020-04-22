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

    RectangleApp {
        id: rectangleApp
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
        switchEnable {
            checked: ControllerThread.enabled
            onCheckedChanged: ControllerThread.enabled = switchEnable.checked
        }

        moreButton {
            action: moreAction
        }

        optionsButton {
            action: optionsAction
        }

        Action {
            id: moreAction
            icon.source: "images/more.png"
            onTriggered: {
                contextMenu.open(rectangleApp.moreButton, contextMenu.exitMenuItem)
            }
        }

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
            if (active) // when options dialog becomes active, hide main window
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

    Timer {
        id: timerReenterSysTray
        interval: 400
        repeat: false
        running: false
    }

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

    onActiveChanged: {
        if (!active)
            hideApp();
    }

    onVisibleChanged: {
        contextMenu.showMenuItem.visible = contextMenu.separatorMenuItem.visible = !visible
    }
}
