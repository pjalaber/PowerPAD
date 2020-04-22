import QtQuick 2.12
import Qt.labs.platform 1.1
import QtQml 2.14
import com.tekit.powerpad.controllerthread 1.0
import com.tekit.powerpad.helper 1.0

SystemTrayIcon
{
    tooltip: qsTr("PowerPAD")
    visible: true
    icon.mask: true    
    icon.source: {
        if (!ControllerThread.enabled)
            "images/icon_disabled.png"
        else
            "images/icon.png"
    }

    menu: contextMenu

    onActivated: {
        // prevent calling this code too fast if main window has just been hidden
        if (timerReenterSysTray.running)
            return

        if (!applicationWindow.visible) // window is minimized to tray => show it
        {
            if (reason !== SystemTrayIcon.Context)
                applicationWindow.showApp(geometry)
        } else if (!applicationWindow.active) { // window is visible but not active
                                                // (can happen if options dialog window is activated) => hide it
            applicationWindow.hideApp()
        }
    }
}
