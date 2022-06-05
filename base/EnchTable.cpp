#include "base/EnchTable.h"

bool operator==(const EnchPlus a, const EnchPlus b)
{
    if(a.name == b.name && a.lvl == b.lvl)
        return true;
    return false;
}

bool operator!=(const EnchPlus a, const EnchPlus b)
{
    if(a.name != b.name || a.lvl != b.lvl)
        return true;
    return false;
}

EnchTable::EnchTable()
{
    mode = StorageMode::AutoSize;
    E_l = DEFAULTLENGTH;
    E = new EnchPlus[E_l];
    E_count = 0;
}

EnchTable::~EnchTable()
{
    delete [] E;
}


EnchPlus *EnchTable::ench(int point)
{
    return &E[point];
}


void EnchTable::setStorageMode(StorageMode m)
{
    mode = m;
    detectSize();
}

EnchTable::StorageMode EnchTable::getStorageMode() const
{
    return mode;
}

void EnchTable::detectSize()
{
    if(mode == StorageMode::AutoSize)
    {
        int rate = E_l/DEFAULTLENGTH;
        if(E_count * 3 > E_l * 2)
        {
            E_l += DEFAULTLENGTH*rate;
            EnchPlus *tm = new EnchPlus[E_l];
            for(int i = 0; i < E_count; i++)
                tm[i] = E[i];
            delete [] E;
            E = tm;
        }
        else if(E_count * 4 < E_l * 1)
        {
            E_l = DEFAULTLENGTH + E_count * 3.0/2;
            EnchPlus *tm = new EnchPlus[E_l];
            for(int i = 0; i < E_count; i++)
                tm[i] = E[i];
            delete [] E;
            E = tm;
        }
    }
    else if(mode == StorageMode::MinimunSize)
    {
        if(E_l != E_count + 1)
        {
            E_l = E_count + 1;
            EnchPlus *tm = new EnchPlus[E_l];
            for(int i = 0; i < E_count; i++)
                tm[i] = E[i];
            delete [] E;
            E = tm;
        }
    }
    else
    {
        return;
    }
}

bool EnchTable::resize(int size)
{
    return true;
}

int EnchTable::size() const
{
    return E_l;
}

int EnchTable::count() const
{
    return E_count;
}


bool EnchTable::add(EnchPlus e)
{
    if(E_count >= E_l)
        return false;
    E[E_count] = e;
    E_count++;
    detectSize();
    return true;
}

bool EnchTable::add()
{
    if(E_count >= E_l)
        return false;
    E[E_count] = e;
    E_count++;
    detectSize();
    return true;
}

bool EnchTable::remove(EnchPlus e)
{
    for(int i = E_count-1; i >= 0; i--)
    {
        if(E[i] == e)
        {
           int j;
           for(j = 1; i + j < E_count; j++)
               E[i] = E[i+j];
           E[i+j-1] = {};
           E_count--;
           detectSize();
           return true;
        }
    }
    return false;
}

bool EnchTable::remove(QString name)
{
    for(int i = E_count-1; i >= 0; i--)
    {
        if(E[i].name == name)
        {
           int j;
           for(j = 1; i + j < E_count; j++)
               E[i] = E[i+j];
           E[i+j-1] = {};
           E_count--;
           detectSize();
           return true;
        }
    }
    return false;
}

void EnchTable::clear()
{
    delete [] E;
    E_l = DEFAULTLENGTH;
    E = new EnchPlus[E_l];
    E_count = 0;
}

