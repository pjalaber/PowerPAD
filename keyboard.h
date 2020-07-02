#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>

class Keyboard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 characterIndex READ characterIndex WRITE setCharacterIndex NOTIFY characterIndexChanged)
    Q_PROPERTY(bool show READ show NOTIFY showChanged)
private:
    static const QString CHARACTERS;
    bool m_show;
    double m_characterIndex;
private:
    explicit Keyboard(QObject *parent = nullptr);
public:
    static Keyboard* instance();
    static double getCharacterIndex(double characterIndex);

    quint32 characterIndex();
    void setCharacterIndex(double characterIndex);
    void incCharacterIndex(double delta);

    Q_INVOKABLE QChar getCharacterAt(qint32 characterIndex);

    bool show();
    void setShow(bool show);

signals:
    void characterIndexChanged();
    void showChanged();
};

#endif // KEYBOARD_H
