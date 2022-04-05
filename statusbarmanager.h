#ifndef STATUSBARMANAGER_H
#define STATUSBARMANAGER_H

#include <QObject>

class StatusBarManager : public QObject
{
    Q_OBJECT
public:
    explicit StatusBarManager(QObject *parent = nullptr);

signals:

};

#endif // STATUSBARMANAGER_H
