import QtQuick 2.14
import Qt.labs.platform 1.1
import com.tekit.powerpad.controllerthread 1.0
import com.tekit.powerpad.helper 1.0

SystemTrayIcon
{
    tooltip: {
        if (!ControllerThread.enabled)
            qsTr("PowerPAD (Disabled)")
        else if (ControllerThread.connectedCount > 0)
            qsTr("PowerPAD (Connected)")
        else
            qsTr("PowerPAD");
    }
    visible: true
    icon.mask: true
    icon.source: {
        if (ControllerThread.state == ControllerThread.StateEnabledWithUI ||
                ControllerThread.state == ControllerThread.StateEnabledWithController ||
                ControllerThread.state == ControllerThread.StateEnabledWithFullscreenExit)
            "images/icon.svg"
        else
            "images/icon_disabled.svg"
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
