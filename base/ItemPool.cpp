#include "base/ItemPool.h"

ItemPool::ItemPool(QObject *parent)
    : QObject{parent}
{
    mode = StorageMode::AutoSize;
    Items_l = DEFAULTLENGTH;
    Items = new Item[Items_l];
    item_count = 0;
}

ItemPool::~ItemPool()
{
    delete [] Items;
}


Item *ItemPool::item(int point)
{
    return &Items[point];
}

int ItemPool::searchWeapon() const
{

}

int ItemPool::thePenaltyBegin(int num) const
{

}

int ItemPool::thePenaltyEnd(int num) const
{

}

void ItemPool::sort(int direction)
{

}


void ItemPool::setStorageMode(StorageMode m)
{
    mode = m;
    detectSize();
}

void ItemPool::detectSize()
{
    if(mode == StorageMode::AutoSize)
    {
        int rate = Items_l/DEFAULTLENGTH;
        if(item_count * 3 > Items_l * 2)
        {
            Items_l += DEFAULTLENGTH*rate;
            Item *tm = new Item[Items_l];
            for(int i = 0; i < item_count; i++)
            {
                tm[i].copy(&Items[i]);
            }
            delete [] Items;
            Items = tm;
        }
        else if(item_count * 4 < Items_l * 1)
        {
            Items_l = DEFAULTLENGTH + item_count * 3.0/2;
            Item *tm = new Item[Items_l];
            for(int i = 0; i < item_count; i++)
            {
                tm[i].copy(&Items[i]);
            }
            delete [] Items;
            Items = tm;
        }
    }
    else if(mode == StorageMode::MinimunSize)
    {
        if(Items_l != item_count + 1)
        {
            Items_l = item_count + 1;
            Item *tm = new Item[Items_l];
            for(int i = 0; i < item_count; i++)
            {
                tm[i].copy(&Items[i]);
            }
            delete [] Items;
            Items = tm;
        }
    }
    else
    {
        return;
    }
}

bool ItemPool::resize(int size)
{
    return true;
}

int ItemPool::size() const
{
    return Items_l;
}

int ItemPool::count() const
{
    return item_count;
}


bool ItemPool::append(Item *it)
{
    if(item_count >= Items_l)
        return false;
    Items[item_count].copy(it);
    item_count++;
    detectSize();
    return true;
}

bool prepend(Item *it);
bool append(Item *it);
void append(QString name, Ench *ench, int dur, int pen);
bool insert(Item *it, int point);
bool replace(Item *it, int point);


bool ItemPool::remove(Item *it)
{
    for(int i = item_count-1; i >= 0; i--)
    {
        if(Items[i].compare(it) == 0)
        {
            int j;
            for(j = 1; i + j < item_count; j++)
                Items[i].copy(&Items[i+j]);
            Items[i+j-1].drop();
            item_count--;
            detectSize();
            return true;
        }
    }
    return false;
}

bool ItemPool::remove(int point)
{
      int j;
      for(j = 1; point + j < item_count; j++)
          Items[point].copy(&Items[point+j]);
      Items[point+j-1].drop();
      item_count--;
      detectSize();
      return true;
}

void ItemPool::clear()
{
    delete [] Items;
    Items_l = DEFAULTLENGTH;
    Items = new Item[Items_l];
    item_count = 0;
}


int ItemPool::maxPenalty() const
{
    int pen = Items[0].penalty;
    for(int i = 1; i < item_count; i++)
        pen = std::max(pen, Items[i].penalty);
    return pen;
}

int ItemPool::minPenalty() const
{
    int pen = Items[0].penalty;
    for(int i = 1; i < item_count; i++)
        pen = std::min(pen, Items[i].penalty);
    return pen;
}

int ItemPool::maxLevelCost() const
{

}

int ItemPool::minLevelCost() const
{

}

int ItemPool::maxCost() const
{

}

int ItemPool::minCost() const
{

}


void ItemPool::fromEnchSet(EnchSet *es, int mode)
{

}

