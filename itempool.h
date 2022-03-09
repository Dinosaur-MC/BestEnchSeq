#ifndef ITEMPOOL_H
#define ITEMPOOL_H

#include <QObject>
#include "basic.h"

enum StorageMode {
    AutoSize,
    FixedSize,
    MinimunSize
};

class ItemPool : public QObject
{
    Q_OBJECT
public:
    explicit ItemPool(QObject *parent = nullptr);

    void setStorageMode(enum StorageMode);
    int detectSize();
    void resize(int size);  //Warning: Be careful, or datas might be lost!

    void insert(Item item, int p);
    void append(Item item);

    void remove(Item item);
    void remove(int p);

    void sort();
    int searchWeapon();
    int count();
    int size();
    Item item(int p);

    int maxPenalty();
    int minPenalty();

    int maxLevelCost();
    int minLevelCost();

    int maxCost();
    int minCost();

    void setForgeMode(int mode);
    static Step preForge(Item A, Item B, int mode); //mode; 0:Normal, 1:Ignore pennalty, 2:Ignore duration, 3:Ignore both
    static Item forge(Item A, Item B);

private:
    int forgeMode;
    StorageMode storage_mode;
    int pool_l, item_count;
    Item *pool;

signals:

};

#endif // ITEMPOOL_H
