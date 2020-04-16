import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.14
import com.tekit.powerpad.controllerthread 1.0

ApplicationWindow {
    id: applicationWindow
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    visible: false
    width: 250
    height: 230

    RectangleApp {
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
    }

    SysTray {
    }

    Timer {
        id: timerHide
        interval: 150
        running: false
        repeat: false
        onTriggered: hide()
    }

    ParallelAnimation {
        id: animation
        SmoothedAnimation {
            id: animationW
            property: "width"
            target: applicationWindow
            duration: 150
        }
        SmoothedAnimation {
            id: animationH
            property: "height"
            target: applicationWindow
            duration: 150
        }
    }

    function screenContainsRect(rect)
    {
        return (rect.left > 0 && rect.left + rect.width < Screen.desktopAvailableWidth &&
                rect.top > 0 && rect.top + rect.height < Screen.desktopAvailableHeight)
    }

    function showApp(nextToRect) {
        if (!timerHide.running)
            showNextToRect(nextToRect)
    }

    function showNextToRect(nextToRect)
    {
        /* try to place window above tray icon */
        var dr = Qt.rect(nextToRect.x + (nextToRect.width / 2) - (width / 2), nextToRect.y - height, width, height)
        if (!screenContainsRect(dr)) {
            /* try to place window right from tray icon */
            dr = Qt.rect(nextToRect.x + nextToRect.width, nextToRect.y - (height / 2), width, height)
            if (!screenContainsRect(dr)) {
                /* try to place window under the tray icon */
                dr = Qt.rect(nextToRect.x + (nextToRect.width / 2) - (width / 2), nextToRect.y + nextToRect.height, width, height)
                if (!screenContainsRect(dr)) {
                    /* try to place window left from tray icon */
                    dr = Qt.rect(nextToRect.x - width, nextToRect.y - (height / 2), width, height)
                }
            }
        }
        x = dr.x
        y = dr.y
        width = 1
        height = 1
        show()
        raise()
        requestActivate()
        animationW.from = 0
        animationW.to = dr.width
        animationH.from = 0
        animationH.to = dr.height
        animation.start()
    }

    onActiveChanged: {
        if (!active)
            timerHide.start()
    }
}
