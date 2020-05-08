#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>

class Keyboard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 characterIndex READ characterIndex WRITE setCharacterIndex NOTIFY characterIndexChanged)
private:
    static const QString CHARACTERS;
    double m_characterIndex;
public:
    explicit Keyboard(QObject *parent = nullptr);
    static Keyboard* instance();
    static double getCharacterIndex(double characterIndex);

    quint32 characterIndex();
    void setCharacterIndex(double characterIndex);
    void incCharacterIndex(double delta);

    Q_INVOKABLE QChar getCharacterAt(qint32 characterIndex);

signals:
    void characterIndexChanged();
};

#endif // KEYBOARD_H
