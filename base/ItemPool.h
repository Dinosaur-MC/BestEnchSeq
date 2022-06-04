#ifndef ITEMPOOL_H
#define ITEMPOOL_H

#include <QObject>
#include "base/Item.h"

class ItemPool : public QObject
{
    Q_OBJECT
public:
    explicit ItemPool(QObject *parent = nullptr);
    ~ItemPool();

    enum StorageMode {
        AutoSize,   //自动大小
        FixedSize,  //固定大小
        MinimunSize //最小大小
    };

    Item *item(int point);
    int searchWeapon() const;
    int thePenaltyBegin(int num) const;
    int thePenaltyEnd(int num) const;
    void sort(int direction);

    void setStorageMode(StorageMode mode);
    bool resize(int size);
    int size() const;
    int count() const;

    bool prepend(Item *it);
    bool append(Item *it);
    void append(QString name, Ench *ench, int dur, int pen);
    bool insert(Item *it, int point);
    bool replace(Item *it, int point);

    bool remove(Item *it);
    bool remove(int point);
    void clear();

    int maxPenalty() const;
    int minPenalty() const;
    int maxLevelCost() const;
    int minLevelCost() const;
    int maxCost() const;
    int minCost() const;

    void fromEnchSet(EnchSet *es, int mode);

private:
    void detectSize();

    const int DEFAULTLENGTH = 32;

    Item *Items;

    StorageMode mode;
    int Items_l;
    int item_count;

};

#endif // ITEMPOOL_H
