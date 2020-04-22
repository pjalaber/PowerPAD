#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QRect>

class Helper : public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = nullptr);
    Q_INVOKABLE void restartApp();
    Q_INVOKABLE QRect computeBestWindowRect(const QString &screenName, const QRect &r,
                                            qint32 windowWidth, qint32 windowHeight);
    static Helper* instance();
signals:

};

#endif // HELPER_H
