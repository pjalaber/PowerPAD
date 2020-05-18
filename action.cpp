#include <xinput.h>
#include "action.h"

Action::Action(QObject *parent) : QObject(parent), m_buttonActionHash(), m_analogActionHash()
{
    m_buttonActionHash.insert(ControllerButtonAction::MouseLeftClick, ControllerButton::A);
    m_buttonActionHash.insert(ControllerButtonAction::MouseRightClick, ControllerButton::X);
    m_buttonActionHash.insert(ControllerButtonAction::Up, ControllerButton::Up);
    m_buttonActionHash.insert(ControllerButtonAction::Down, ControllerButton::Down);
    m_buttonActionHash.insert(ControllerButtonAction::Right, ControllerButton::Right);
    m_buttonActionHash.insert(ControllerButtonAction::Left, ControllerButton::Left);
    m_buttonActionHash.insert(ControllerButtonAction::Backspace, ControllerButton::B);
    m_buttonActionHash.insert(ControllerButtonAction::Enter, ControllerButton::Start);
    m_buttonActionHash.insert(ControllerButtonAction::KeyboardActivate, ControllerButton::LB);
    m_buttonActionHash.insert(ControllerButtonAction::KeyboardEnterKey, ControllerButton::A);

    m_analogActionHash.insert(ControllerAnalogAction::MouseMove, ControllerAnalog::Left);
    m_analogActionHash.insert(ControllerAnalogAction::MouseWheel, ControllerAnalog::Right);
    m_analogActionHash.insert(ControllerAnalogAction::KeyboardScroll, ControllerAnalog::Left);

    m_comboActionHash.insert(ControllerComboAction::EnableDisableApp,
                             new QPair<ControllerButton, ControllerButton>(ControllerButton::Back, ControllerButton::Start));
}

Action* Action::instance()
{
    static Action *a = new Action();
    return a;
}

Action::ControllerButton Action::find(ControllerButtonAction action)
{
    return m_buttonActionHash.value(action, ControllerButton::None);
}

Action::ControllerAnalog Action::find(ControllerAnalogAction action)
{
    return m_analogActionHash.value(action, ControllerAnalog::None);
}

const QPair<Action::ControllerButton, Action::ControllerButton> *Action::find(ControllerComboAction action)
{
    return m_comboActionHash.value(action, nullptr);
}

quint32 Action::getXInputButton(ControllerButton b)
{
    switch (b) {
    case ControllerButton::A:
        return XINPUT_GAMEPAD_A;
    case ControllerButton::B:
        return XINPUT_GAMEPAD_B;
    case ControllerButton::X:
        return XINPUT_GAMEPAD_X;
    case ControllerButton::Y:
        return XINPUT_GAMEPAD_Y;
    case ControllerButton::Left:
        return XINPUT_GAMEPAD_DPAD_LEFT;
    case ControllerButton::Right:
        return XINPUT_GAMEPAD_DPAD_RIGHT;
    case ControllerButton::Up:
        return XINPUT_GAMEPAD_DPAD_UP;
    case ControllerButton::Down:
        return XINPUT_GAMEPAD_DPAD_DOWN;
    case ControllerButton::Start:
        return XINPUT_GAMEPAD_START;
    case ControllerButton::Back:
        return XINPUT_GAMEPAD_BACK;
    case ControllerButton::LT:
        return XINPUT_GAMEPAD_LEFT_THUMB;
    case ControllerButton::RT:
        return XINPUT_GAMEPAD_RIGHT_THUMB;
    case ControllerButton::LS:
        return XINPUT_GAMEPAD_LEFT_SHOULDER;
    case ControllerButton::RS:
        return XINPUT_GAMEPAD_RIGHT_SHOULDER;
    case ControllerButton::LB:
        return XINPUT_GAMEPAD_LEFT_THUMB;
    case ControllerButton::RB:
        return XINPUT_GAMEPAD_RIGHT_THUMB;
    case ControllerButton::None:
        return 0;
    default:
        Q_ASSERT(0);
        return 0;
    }
}
