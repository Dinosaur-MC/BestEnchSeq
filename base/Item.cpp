#include "base/Item.h"

Item::Item()
{
    ench = new EnchSet();
}

Item::~Item()
{
    delete ench;
}


void Item::copy(Item *it)
{
    name = it->name;
    ench->copy(it->ench);
    durability = it->durability;
    penalty = it->penalty;
}

int Item::compare(Item *it)
{
    int d = 0;
    if(name == it->name && durability == it->durability && penalty == it->penalty)
        d = ench->compare(it->ench);
    else
        d = -1;
    return d;
}

void Item::drop()
{
    name = "";
    ench->clear();
    durability = 0;
    penalty = 0;
}


Item *Item::forge(Item it)
{

}

int Item::forge(Item it, int m)
{

}

