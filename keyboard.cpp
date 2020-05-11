#include "keyboard.h"

const QString Keyboard::CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

Keyboard::Keyboard(QObject *parent) : QObject(parent),
    m_show(false), m_characterIndex(0)
{
}

Keyboard* Keyboard::instance()
{
    static Keyboard *s = new Keyboard();
    return s;
}

quint32 Keyboard::characterIndex()
{
    return (quint32)m_characterIndex;
}

void Keyboard::incCharacterIndex(double delta)
{
    setCharacterIndex(m_characterIndex + delta);
}

void Keyboard::setCharacterIndex(double characterIndex)
{
    if (characterIndex < 0)
        characterIndex += CHARACTERS.length();
    else if (characterIndex > CHARACTERS.length())
        characterIndex -= CHARACTERS.length();

    bool emitSignal = (quint32)characterIndex != (quint32)m_characterIndex;
    m_characterIndex = characterIndex;
    if (emitSignal)
        emit characterIndexChanged();
}

QChar Keyboard::getCharacterAt(qint32 characterIndex)
{
    characterIndex %= CHARACTERS.length();
    if (characterIndex < 0)
        characterIndex += CHARACTERS.length();
    return CHARACTERS[characterIndex];
}

bool Keyboard::show()
{
    return m_show;
}

void Keyboard::setShow(bool show)
{
    if (m_show != show) {
        m_show = show;
        emit showChanged();
    }
}
