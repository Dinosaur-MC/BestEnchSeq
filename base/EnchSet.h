#ifndef ENCHSET_H
#define ENCHSET_H

#include <QObject>
#include "base/Ench.h"

class EnchSet : public QObject
{
    Q_OBJECT

public:
    explicit EnchSet();
    ~EnchSet();

    enum StorageMode {
        AutoSize,   //自动大小
        FixedSize,  //固定大小
        MinimunSize //最小大小
    };

    Ench *ench(int point);

    void setStorageMode(StorageMode m);
    StorageMode getStorageMode() const;
    void detectSize();
    bool resize(int size);
    int size() const;
    int count() const;

    bool add(Ench e);
    bool remove(Ench e);
    bool remove(QString name);
    void clear();

    void copy(EnchSet* es);
    int compare(EnchSet* es);

private:
    const int DEFAULTLENGTH = 16;
    Ench *E;

    StorageMode mode;
    int E_l;
    int E_count;

};

#endif // ENCHSET_H
