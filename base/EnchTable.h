#ifndef ENCHTABLE_H
#define ENCHTABLE_H

#include <QObject>
#include "base/Config.h"

struct EnchPlus {
    MCE edition;    //魔咒适用版本
    QString name;   //魔咒名称
    int max_level;  //魔咒最高等级
    int poor_max_level; //30级附魔台附魔一般最高可获得等级及宝藏魔咒（实际上用于BasicEBook物品配置中，是魔咒的最大可直接获得的魔咒等级，可自定义）
    int multiplier[2];  //魔咒乘数，[0]为物品乘数，[1]为附魔书乘数
    QString repulsion[64]; //冲突的魔咒
    int suitable[64];  //适配的物品
};

bool operator==(const EnchPlus a, const EnchPlus b);
bool operator!=(const EnchPlus a, const EnchPlus b);

class EnchTable : public QObject
{
    Q_OBJECT

public:
    explicit EnchTable();
    ~EnchTable();

    enum StorageMode {
        AutoSize,   //自动大小
        FixedSize,  //固定大小
        MinimunSize //最小大小
    };

    EnchPlus *ench(int point);

    void setStorageMode(StorageMode m);
    StorageMode getStorageMode() const;
    void detectSize();
    bool resize(int size);
    int size() const;
    int count() const;

    bool add(EnchPlus e);
    bool add();
    bool remove(EnchPlus e);
    bool remove(QString name);
    void clear();

private:
    const int DEFAULTLENGTH = 64;
    EnchPlus *E;

    StorageMode mode;
    int E_l;
    int E_count;

};

#endif // ENCHTABLE_H

