import QtQuick 2.0
import QtQuick.Window 2.14
import com.tekit.powerpad.keyboard 1.0

Window {
    flags: Qt.Tool | Qt.FramelessWindowHint| Qt.WindowStaysOnTopHint | Qt.WindowTransparentForInput
    minimumWidth: keyboardItem.width
    minimumHeight: keyboardItem.height
    maximumWidth: minimumWidth
    maximumHeight: minimumHeight
    x: (screen.width - width) / 2
    y: (screen.height - height) / 2
    color: "transparent"
    visible: true
    KeyboardItem {
        id: keyboardItem
        visible: false
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
            console.log("keyboard show changed")
            if (Keyboard.show)
                keyboardItem.visible = true
            else
                keyboardItem.visible = false
        }
    }

    Component.onCompleted: {
        show()
    }
}
