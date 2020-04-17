import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.14
import com.tekit.powerpad.controllerthread 1.0
import com.tekit.powerpad.helper 1.0

ApplicationWindow {
    id: applicationWindow
    flags: Qt.FramelessWindowHint
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

    function showApp(nextToRect) {
        if (!timerHide.running)
            showNextToRect(nextToRect)
    }

    function showNextToRect(nextToRect)
    {
        var dr = Helper.computeBestWindowRect(screen.name, nextToRect, width, height);
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
