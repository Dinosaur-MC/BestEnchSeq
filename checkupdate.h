#ifndef CHECKUPDATE_H
#define CHECKUPDATE_H

#include <QObject>

class CheckUpdate : public QObject
{
    Q_OBJECT
public:
    explicit CheckUpdate(QObject *parent = nullptr);

signals:

};

#endif // CHECKUPDATE_H
