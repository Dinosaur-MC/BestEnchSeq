#ifndef ITEMPOOL_H
#define ITEMPOOL_H

#include <QObject>
#include "basic.h"

enum StorageMode {
    AutoSize,
    FixedSize,
    MinimunSize
};

enum ForgeMode {
    Normal,
    IgnoreFixing,
    IgnorePenalty,
    IgnoreFixing_Penalty
};

int combine(QString ench, int a, int b);

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
    void replace(Item item, int p);

    void remove(Item item);
    void remove(int p);

    void sort();
    int searchWeapon();
    int penaltyAreaBegin(int pen);
    int penaltyAreaEnd(int pen);
    int count();
    int size();
    Item item(int p);

    int maxPenalty();
    int minPenalty();

    int maxLevelCost();
    int minLevelCost();

    int maxCost();
    int minCost();

    void setForgeMode(ForgeMode mode);
    static Step preForge(Item A, Item B, ForgeMode mode); //mode; 0:Normal, 1:Ignore pennalty, 2:Ignore duration, 3:Ignore both
    static Item forge(Item A, Item B);

private:
    ForgeMode forgeMode;
    StorageMode storage_mode;
    int pool_l, item_count;
    Item *pool;

signals:

};

#endif // ITEMPOOL_H
