#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H

#include <QObject>
#include "core.h"

class TableManager : public QObject
{
    Q_OBJECT
public:
    explicit TableManager(QObject *parent = nullptr);

    bool newTable(QString fname, int tmp_table = -1);
    bool loadDefaultTable();
    bool loadTable(QString fname);
    void unloadTable(QString fname);
    int loadedCount();
    void empty();

    void setCursor(int num);

    DataTable getTable();
    void optimize();
    void optimizeAll();

    void insertEnch(int p, EnchData ench);
    EnchData takeEnch(int p);
    void insertGroup(int p, Group group);
    Group takeGroup(int p);

    bool exportTable(QString path = "./tables/");
    bool apply();
    void unloadTable();

private:
    int findTable(QString fname);
    bool checkCursor();
    bool checkCursor(int cur);

    int cursor;
    DataTableList table_list;

signals:
};

#endif // TABLEMANAGER_H
