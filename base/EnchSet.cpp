#include "base/EnchSet.h"

EnchSet::EnchSet()
{
    mode = StorageMode::AutoSize;
    E_l = DEFAULTLENGTH;
    E = new Ench[E_l];
    E_count = 0;
}

EnchSet::~EnchSet()
{
    delete [] E;
}


Ench *EnchSet::ench(int point)
{
    return &E[point];
}


void EnchSet::setStorageMode(StorageMode m)
{
    mode = m;
    detectSize();
}

EnchSet::StorageMode EnchSet::getStorageMode() const
{
    return mode;
}

void EnchSet::detectSize()
{
    if(mode == StorageMode::AutoSize)
    {
        int rate = E_l/DEFAULTLENGTH;
        if(E_count * 3 > E_l * 2)
        {
            E_l += DEFAULTLENGTH*rate;
            Ench *tm = new Ench[E_l];
            for(int i = 0; i < E_count; i++)
                tm[i] = E[i];
            delete [] E;
            E = tm;
        }
        else if(E_count * 4 < E_l * 1)
        {
            E_l = DEFAULTLENGTH + E_count * 3.0/2;
            Ench *tm = new Ench[E_l];
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
            Ench *tm = new Ench[E_l];
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

bool EnchSet::resize(int size)
{
    return true;
}

int EnchSet::size() const
{
    return E_l;
}

int EnchSet::count() const
{
    return E_count;
}


bool EnchSet::add(Ench e)
{
    if(E_count >= E_l)
        return false;
    E[E_count] = e;
    E_count++;
    detectSize();
    return true;
}

bool EnchSet::remove(Ench e)
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

bool EnchSet::remove(QString name)
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

void EnchSet::clear()
{
    delete [] E;
    E_l = DEFAULTLENGTH;
    E = new Ench[E_l];
    E_count = 0;
}

void EnchSet::copy(EnchSet* es)
{
    delete [] E;
    mode = es->getStorageMode();
    E_l = es->size();
    E_count = es->count();
    E = new Ench[E_l];
    for(int i = 0; i < E_count; i++)
        E[i] = *es->ench(i);
}

int EnchSet::compare(EnchSet* es)
{
    int d = 0;
    if(count() == es->count())
    {
        for(int i = 0; i < count(); i++)
        {
            if(ench(i) != es->ench(i))
                d++;
        }
    }
    else
        d = -1;
    return d;
}

