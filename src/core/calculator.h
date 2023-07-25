#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QThread>
#include "core.h"
#include <QTimer>
#include <QDateTime>

// 魔咒 {物品, 池}
struct _Ench {
    int id;   // 魔咒ID
    int lvl;    // 魔咒等级
};

struct _EnchPlus {
    int id;   // 魔咒ID
    MCE edition;    // 魔咒适用版本
    int max_level;  // 魔咒最高等级
    int poor_max_level; // 30级附魔台附魔一般最高可获得等级及宝藏魔咒（实际上用于BasicEBook物品配置中，是魔咒的最大可直接获得的魔咒等级，可自定义）
    int multiplier[2];  // 魔咒乘数，[0]为物品乘数，[1]为附魔书乘数
    QVector<int> repulsion; // 冲突的魔咒
};

// 武器 池
struct _Weapon {
    int id;   // 武器ID
    QVector<_EnchPlus> suitableE;    // 适配的魔咒
};

// 物品
struct _Item {
    int id; // 物品ID
    ItemType type;   // 物品类型
    QVector<_Ench> ench; // 物品魔咒
    int durability;   // 物品耐久
    int penalty;    // 惩罚数
};

// 流程
struct _FlowStep {
    _Item a; // 目标物品
    _Item b; // 牺牲物品
    int levelCost;   // 等级花费
    int pointCost;   // 经验值花费
};


// 数学运算
int penaltyCost(int x);
int duplicationSeries(int x);
QVector<int> dupFloorMembers(int x, int n);


class Calculator : public QThread
{
    Q_OBJECT
public:
    explicit Calculator(Core *c, QObject *parent = nullptr);
    ~Calculator();
    void initialize();

    // 格式转换函数
    _Ench formatToLocal(const Ench &e);
    _EnchPlus formatToLocal(const EnchPlus &ep);
    _Weapon formatToLocal(const WeaponPlus &wp);
    _Item formatToLocal(const Item &it);
    _FlowStep formatToLocal(const FlowStep &fs);

    Item formatToCore(const _Item &it);
    FlowStep formatToCore(const _FlowStep &fs);

    // 基本操作函数
    void setEdition(MCE v);
    void setICM(ICM v);
    void setALGM(ALGM v);
    void setPFADDN(PFADDN v);

    void setInputItem(const _Item &it);
    void setInputItem(const Item &it);
    void setOutputItem(const _Item &it);
    void setOutputItem(const Item &it);

    void setItemPool(const QVector<_Item> &ip);
    void setItemPool(const QVector<Item> &ip);

    _Item poolAt(int p);
    void appendInPool(const _Item &it);
    void appendInPool(const Item &it);
    void prependInPool(const _Item &it);
    void prependInPool(const Item &it);
    void removeFromPool(int p, int n = 1);
    void removePoolFirst();
    void removePoolLast();
    void clearPool();   // 清空物品池
    int poolItemCount();    // 返回物品池内物品数量

    bool optimizePool();    // 检查目标可行性及优化物品池
    void sortPool();    // 排序物品，按简单附魔花费升序

    static int getContentIndex(const QVector<int> ns, const int &n);    // 内容查找
    static int getContentIndex(const QVector<_Ench> &es, const _Ench &e, bool strict = true);

    void getResult();    // 获取计算结果

    // 基本运算函数
    int integratingDegree(const _Item &it); // 计算物品契合度
    bool checkCombinable(const _Item &it, const _Ench &e, bool strict = true);  // 检查锻造可行性

    int preForge(const _Item &a, const _Item &b, bool strict = true);   // 计算物品锻造花费
    int preForge(const _Item &it, bool strict = true);
    _Item forge(const _Item &a, const _Item &b);    // 锻造物品
    _FlowStep forge(_Item *a, const _Item &b);

private:
    MDebug mdb;
    Core *core;

    // 变量
    MCE edition;
    ICM icm;
    ALGM algm;
    PFADDN pfaddn;

    QVector<_Weapon> wps;
    QVector<_EnchPlus> eps;
    _Item input_item;
    _Item output_item;

    QVector<_Item> item_pool;
    QVector<_Item> dropped_item;

    QDateTime start_t;
    QDateTime end_t;
    QTimer timer;

    QVector<_FlowStep> flow;

    // 排序函数
    void _sort_1(int left, int right);
    void sortPool(QVector<_Item> *its, int left, int right);

    // 算法函数
    void alg_0();
    void alg_1();
    void alg_2();

protected:
   virtual void run();  // 执行函数，使用 start() 开始运行新线程

signals:

};

#endif // CALCULATOR_H
