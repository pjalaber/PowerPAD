#ifndef WINSYS_H
#define WINSYS_H

#include <QObject>
#include <QLibrary>
#include <QPoint>
#include <xinput.h>
#include <windows.h>

class WinSys : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)

private:
    QLibrary m_xinputLib, m_shellLib;

public:
    enum Status
    {
        StatusOK,
        StatusXInputLibraryNotFound,
        StatusXInputSymbolNotFound,
        StatusShell32NotFound,
        StatusShell32SymbolNotFound
    };
    Q_ENUM(Status)

protected:
    Status m_status;

private:
    explicit WinSys(QObject *parent = nullptr);
public:
    static WinSys* instance();

    static void sendUnicodeKeyDown(quint16 unicodeKey);
    static void sendUnicodeKeyUp();
    static void sendVirtualKeyDown(quint16 unicodeKey);
    static void sendVirtualKeyUp();
    static void sendKeyDown(bool isVirtual, quint16 unicodeKey);
    static void sendKeyUp(bool isVirtual);

    static void sendMouseWheel(qint32 scroll);

    enum class MouseButton
    {
        Down, Up, None
    };
    static void sendMouseButton(MouseButton leftButton, MouseButton rightButton);
    static bool getMouseCursorPos(QPoint &p);
    static void setMouseCursorPos(const QPoint &p);

    static bool isFullScreenMouseCursorHidden();
    static quint32 XInputGetState(DWORD controllerIndex, XINPUT_STATE *xinput);

    Status status();
    void setStatus(Status status);

signals:
    void statusChanged();
};

#endif // WINSYS_H
