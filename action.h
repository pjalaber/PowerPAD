#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include <QHash>
#include <QPair>

class Action : public QObject
{
    Q_OBJECT    
public:
    enum class ControllerButtonAction
    {
        MouseLeftClick, MouseRightClick,
        Left, Right, Up, Down, Backspace, Enter,
        KeyboardActivate, KeyboardEnterKey,
    };

    enum class ControllerAnalogAction
    {
        MouseMove, MouseWheel,
        KeyboardScroll,
    };

    enum class ControllerComboAction
    {
        EnableDisableApp
    };

    enum class ControllerButton
    {
        None,
        Up, Down, Left, Right, Start, Back, LT, RT, LB, RB, LS, RS, A, B, X, Y
    };

    enum class ControllerAnalog
    {
        None,
        Left, Right
    };

protected:
    QHash<ControllerButtonAction, ControllerButton> m_buttonActionHash;
    QHash<ControllerAnalogAction, ControllerAnalog> m_analogActionHash;
    QHash<ControllerComboAction, QPair<ControllerButton, ControllerButton>*> m_comboActionHash;

private:
    explicit Action(QObject *parent = nullptr);

public:
    static Action* instance();
    ControllerButton find(ControllerButtonAction action);
    ControllerAnalog find(ControllerAnalogAction action);
    const QPair<ControllerButton, ControllerButton> *find(ControllerComboAction action);
    static quint32 getXInputButton(ControllerButton b);
signals:

};

inline uint qHash(Action::ControllerButtonAction key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

inline uint qHash(Action::ControllerAnalogAction key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}

inline uint qHash(Action::ControllerComboAction key, uint seed)
{
    return ::qHash(static_cast<uint>(key), seed);
}


#endif // ACTION_H
