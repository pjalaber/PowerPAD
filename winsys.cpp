#include <QDebug>
#include <windows.h>
#include "winsys.h"

typedef DWORD (WINAPI *XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE *pState);
static XInputGetState_t XInputGetStateL = nullptr;

typedef enum {
    QUNS_NOT_PRESENT = 1,
    QUNS_BUSY = 2,
    QUNS_RUNNING_D3D_FULL_SCREEN = 3,
    QUNS_PRESENTATION_MODE = 4,
    QUNS_ACCEPTS_NOTIFICATIONS = 5,
    QUNS_QUIET_TIME = 6,
    QUNS_APP = 7
} QUERY_USER_NOTIFICATION_STATE;

typedef HRESULT (WINAPI *SHQueryUserNotificationState_t)(QUERY_USER_NOTIFICATION_STATE *pquns);
static SHQueryUserNotificationState_t SHQueryUserNotificationStateL = nullptr;

WinSys::WinSys(QObject *parent) : QObject(parent), m_status(StatusOK)
{
    m_xinputLib.setFileName(QStringLiteral("xinput1_4.dll"));
    if (!m_xinputLib.load()) {
        m_xinputLib.setFileName(QStringLiteral("xinput1_3.dll"));
        m_xinputLib.load();
    }

    if (!m_xinputLib.isLoaded()) {
        setStatus(StatusXInputLibraryNotFound);
        return;
    }

    XInputGetStateL = (XInputGetState_t) m_xinputLib.resolve("XInputGetState");
    if (XInputGetStateL == nullptr) {
        setStatus(StatusXInputSymbolNotFound);
        return;
    }

    m_shellLib.setFileName(QStringLiteral("shell32.dll"));
    if (!m_shellLib.load()) {
        setStatus(StatusShell32NotFound);
        return;
    }

    SHQueryUserNotificationStateL = (SHQueryUserNotificationState_t)m_shellLib.resolve("SHQueryUserNotificationState");
    if (SHQueryUserNotificationStateL == nullptr)
    {
        setStatus(StatusShell32SymbolNotFound);
        return;
    }
}

WinSys* WinSys::instance()
{
    static WinSys *w = new WinSys();
    return w;
}

void WinSys::sendVirtualKeyDown(quint16 virtualKey)
{
    INPUT input = {};

    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = 0;
    input.ki.wVk = virtualKey;
    SendInput(1, &input, sizeof(input));
}

void WinSys::sendVirtualKeyUp()
{
    INPUT input = {};

    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(input));
}

void WinSys::sendUnicodeKeyDown(quint16 unicodeKey)
{
    INPUT input = {};

    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_UNICODE;
    input.ki.wScan = unicodeKey;
    SendInput(1, &input, sizeof(input));
}

void WinSys::sendUnicodeKeyUp()
{
    INPUT input = {};

    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(input));
}

void WinSys::sendKeyDown(bool isVirtual, quint16 key)
{
    if (isVirtual)
        sendVirtualKeyDown(key);
    else
        sendUnicodeKeyDown(key);
}

void WinSys::sendKeyUp(bool isVirtual)
{
    if (isVirtual)
        sendVirtualKeyUp();
    else
        sendUnicodeKeyUp();
}

void WinSys::sendMouseWheel(qint32 scroll)
{
    INPUT input = {};

    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = scroll;
    SendInput(1, &input, sizeof(input));
}

void WinSys::sendMouseButton(MouseButton leftButton, MouseButton rightButton)
{
    INPUT input = {};

    switch (leftButton) {
    case MouseButton::Down:
        input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
        break;
    case MouseButton::Up:
        input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
        break;
    case MouseButton::None:
        break;
    }

    switch (rightButton) {
    case MouseButton::Down:
        input.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
        break;
    case MouseButton::Up:
        input.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
        break;
    case MouseButton::None:
        break;
    }

    if (input.mi.dwFlags > 0) {
        input.type = INPUT_MOUSE;
        SendInput(1, &input, sizeof(input));
    }
}

void WinSys::moveMouse(double deltax, double deltay)
{
    INPUT input = {};
    POINT p;

    GetCursorPos(&p);

    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
    input.mi.dx = (p.x + deltax) / GetSystemMetrics(SM_CXVIRTUALSCREEN) * 65535;
    input.mi.dy = (p.y - deltay) / GetSystemMetrics(SM_CYVIRTUALSCREEN) * 65535;
    SendInput(1, &input, sizeof(input));
}

WinSys::Status WinSys::status()
{
    return m_status;
}

void WinSys::setStatus(Status status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

bool WinSys::isFullScreenMouseCursorHidden()
{
    CURSORINFO c;
    QUERY_USER_NOTIFICATION_STATE quns;
    Q_ASSERT(SHQueryUserNotificationStateL != nullptr);

    SHQueryUserNotificationStateL(&quns);
    if (quns != QUNS_RUNNING_D3D_FULL_SCREEN && quns != QUNS_BUSY)
        return false;

    c.cbSize = sizeof(CURSORINFO);
    if (!GetCursorInfo(&c))
        return false;

    return c.flags != CURSOR_SHOWING;
}

quint32 WinSys::XInputGetState(DWORD controllerIndex, XINPUT_STATE *xinput)
{
    Q_ASSERT(XInputGetStateL != nullptr);
    return XInputGetStateL(controllerIndex, xinput);
}
