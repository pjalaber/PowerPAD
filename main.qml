import QtQuick 2.12
import QtQuick.Controls 2.13

ApplicationWindow {
    id: applicationWindow
    flags: Qt.Tool | Qt.FramelessWindowHint
    visible: false
    width: 300
    height: 300

    RectangleApp {
        id: rectangleApp
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

    function screenContainsRect(rect)
    {
        return (rect.left > 0 && rect.left + rect.width < screen.width &&
                rect.top > 0 && rect.top + rect.height < screen.height)
    }

    function showApp(sysTrayIconRect) {
        if (!timerHide.running) {
            show()
            moveNextToTray(sysTrayIconRect)
            raise()
            requestActivate()
        }
    }

    function moveNextToTray(sysTrayIconRect)
    {
        /* try to place window above tray icon */
        var r = Qt.rect(sysTrayIconRect.x + (sysTrayIconRect.width / 2) - (width / 2), sysTrayIconRect.y - height, width, height)
        if (!screenContainsRect(r)) {
            /* try to place window right from tray icon */
            r = Qt.rect(sysTrayIconRect.x + sysTrayIconRect.width, sysTrayIconRect.y - (height / 2), width, height)
            if (!screenContainsRect(r)) {
                /* try to place window under the tray icon */
                r = Qt.rect(sysTrayIconRect.x + (sysTrayIconRect.width / 2) - (width / 2), sysTrayIconRect.y + sysTrayIconRect.height, width, height)
                if (!screenContainsRect(r)) {
                    /* try to place window left from tray icon */
                    r = Qt.rect(sysTrayIconRect.x - width, sysTrayIconRect.y - (height / 2), width, height)
                }
            }
        }
        x = r.left
        y = r.top
    }

    onActiveChanged: {
        if (!active)
            timerHide.start()
    }
}
