#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QRect>

class Helper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString aboutText READ aboutText NOTIFY aboutTextChanged)
public:
    explicit Helper(QObject *parent = nullptr);
    static Helper* instance();

    Q_INVOKABLE void restartApp();

    Q_INVOKABLE QRect computeBestWindowRect(const QString &screenName, const QRect &r,
                                            qint32 windowWidth, qint32 windowHeight);
    QString aboutText();

signals:
    void aboutTextChanged();
};

#endif // HELPER_H
