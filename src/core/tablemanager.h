#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H

#include <QObject>
#include "core.h"

class TableManager : public QObject
{
    Q_OBJECT
public:
    explicit TableManager(QObject *parent = nullptr);

    void scanTable();
    void disableTable(QString fname);
    void enableTable(QString fname);
    bool newTable(QString fname, int tmp_table = -1);

    bool loadInternalTable();
    bool loadTable(QString fname);
    void unloadTable(QString fname);
    bool reloadTable(int index);
    int count() const;
    void empty();

    void setCursor(int num);

    QList<DataTableInfo> getAllTabeInfo() const;
    DataTableInfo getTabeInfo() const;
    DataTable getTable() const;
    void optimize();
    void optimizeAll();

    void insertEnch(int p, const EnchData &ench);
    void replaceEnch(int p, const EnchData &ench);
    EnchData takeEnch(int p);
    void insertGroup(int p, const Group &group);
    void replaceGroup(int p, const Group &group);
    Group takeGroup(int p);

    bool exportTable(QString path = "./tables/");
    void unloadTable();
    bool apply();

    int findTable(QString fname) const;

private:
    bool checkCursor() const;
    bool checkCursor(int cur) const;

    int cursor;
    DataTableList table_list;

signals:
    void cursorChanged();
    void tableUpdated();

    void tableContentChanged(int t, int p);
};

extern TableManager global_table_mgr;

#endif // TABLEMANAGER_H
