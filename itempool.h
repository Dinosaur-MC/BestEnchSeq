#ifndef ITEMPOOL_H
#define ITEMPOOL_H

#include <QObject>
#include "base.h"
//#include "datamanager.h"

enum StorageMode {
    AutoSize,   //自动大小
    FixedSize,  //固定大小
    MinimunSize //最小大小
};

enum ForgeMode {
    Normal, //常规模式
    IgnoreFixing,   //忽略修复
    IgnorePenalty,  //忽略惩罚
    IgnoreFixing_Penalty    //忽略修复和惩罚
};

int combine(QString ench, int a, int b);

class ItemPool : public QObject //物品池类，存储物品信息
{
    Q_OBJECT
public:
    explicit ItemPool();

    void setStorageMode(enum StorageMode);  //设置池存储模式
    int detectSize();   //检查存储情况并自动调整池大小
    void resize(int size);  //重新设置池大小（Warning: Be careful, or datas might be lost!）

    void insert(Item item, int p);  //在p位置插入物品
    void append(Item item); //追加物品
    void append(QString name, Ench *ench, int ench_l, int dur, int pen);
    void replace(Item item, int p); //替换p位置的物品

    void remove(Item item); //移除物品
    void remove(int p); //移除p位置的物品
    void clear();   //清空池

    void sort();    //排序物品
    int searchWeapon(); //搜索武器并返回结果位置或未找到时返回-1
    int penaltyAreaBegin(int pen);  //惩罚值区域开始，需使用前进行排序
    int penaltyAreaEnd(int pen);    //惩罚值区域关闭，需使用前进行排序
    int count();    //物品数量
    int size(); //池大小
    Item item(int p);   //返回p位置的物品

    int maxPenalty();   //获取惩罚最大值
    int minPenalty();   //获取惩罚最小值

    int maxLevelCost(); //获取魔咒花费最大值
    int minLevelCost(); //获取魔咒花费最小值

    int maxCost();  //获取花费最大值
    int minCost();  //获取花费最小值

    void setForgeMode(ForgeMode mode);  //设置锻造模式

    void cloneFrom(ItemPool *src);   //复制物品池内容

    static Step preForge(Item A, Item B, ForgeMode mode); //预锻造，返回流程步骤，包含锻造花费（mode; 0:Normal, 1:Ignore pennalty, 2:Ignore duration, 3:Ignore both）
    static Item forge(Item A, Item B);  //锻造合并物品

private:
    ForgeMode forgeMode;
    StorageMode storage_mode;
    int item_count; //物品数量
    int pool_l; //池大小
    Item *pool;

signals:

};

#endif // ITEMPOOL_H
