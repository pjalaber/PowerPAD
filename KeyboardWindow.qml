import QtQuick 2.0
import QtQuick.Window 2.14
import com.tekit.powerpad.keyboard 1.0
import com.tekit.powerpad.helper 1.0

Window {
    flags: Qt.FramelessWindowHint| Qt.WindowStaysOnTopHint | Qt.WindowTransparentForInput | Qt.Popup | Qt.NoDropShadowWindowHint
    minimumWidth: keyboardItem.width
    minimumHeight: keyboardItem.height
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    x: (screen.width - width) / 2
    y: (screen.height - height) / 2
    color: "transparent"
    visible: false
    KeyboardItem {
        id: keyboardItem
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

    Connections {
        target: Keyboard
        onShowChanged: {
            if (Keyboard.show) {
                Helper.saveForegroundWindow()
                show()
                Helper.restoreForegroundWindow()
            }
            else
                hide()
        }
    }
}
