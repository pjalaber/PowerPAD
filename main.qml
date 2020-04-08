import QtQuick 2.12
import QtQuick.Controls 2.13

ApplicationWindow {
    Loader {
        source: "qrc:/systray.qml"
    }

    Rectangle {
        id: rectangle
        color: "#ffffff"
        anchors.fill: parent
        border.width: 1

        Row {
            id: row
            anchors.fill: parent
        }

        Switch {
            id: element
            x: 8
            y: 42
            text: qsTr("Enabled")
        }

        Label {
            id: label
            x: 46
            y: 1
            width: 254
            height: 33
            text: qsTr("PowerPAD")
            verticalAlignment: Text.AlignVCenter
        }

        Image {
            id: image
            x: 1
            y: 1
            width: 46
            height: 33
            source: "icon.png"
            fillMode: Image.PreserveAspectFit
        }
    }
    id: applicationWindow
    flags: Qt.Tool | Qt.FramelessWindowHint
    title: qsTr("PowerPAD")
    width: 300
    height: 300
    visible: false

    Timer {
        id: timerHide
        interval: 150
        running: false
        repeat: false
        onTriggered: applicationWindow.hide()
    }

    function screenContainsRect(rect)
    {
        return (rect.left > 0 && rect.left + rect.width < screen.width &&
               rect.top > 0 && rect.top + rect.height < screen.height)
    }

    function showApp(sysTrayIconRect) {
        if (!timerHide.running) {
            /* try to place window above tray icon */
            var r = Qt.rect(sysTrayIconRect.x - (width / 2), sysTrayIconRect.y - height, width, height)
            if (!screenContainsRect(r)) {
                /* try to place window right from tray icon */
                r = Qt.rect(sysTrayIconRect.x + sysTrayIconRect.width, sysTrayIconRect.y - (height / 2), width, height)
                if (!screenContainsRect(r)) {
                    /* try to place window under the tray icon */
                    r = Qt.rect(sysTrayIconRect.x - (width / 2), sysTrayIconRect.y + sysTrayIconRect.height, width, height)
                    if (!screenContainsRect(r)) {
                        /* try to place window left from tray icon */
                        r = Qt.rect(sysTrayIconRect.x - width, sysTrayIconRect.y - (height / 2), width, height)
                    }
                }
            }

            x = r.left
            y = r.top
            show()
            raise()
            requestActivate()
        }
    }

    function hideApp() {
        timerHide.start()
    }

    onActiveChanged: {
       if (!active)
            hideApp()
    }
}


/*##^##
Designer {
    D{i:3;anchors_height:400;anchors_width:200;anchors_x:57;anchors_y:27}D{i:2;anchors_height:208;anchors_width:200;anchors_x:67;anchors_y:58}
}
##^##*/
