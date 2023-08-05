#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H

#include <QObject>
#include "core.h"

class TableManager : public QObject
{
    Q_OBJECT
public:
    explicit TableManager(QObject *parent = nullptr);

signals:
};

#endif // TABLEMANAGER_H
