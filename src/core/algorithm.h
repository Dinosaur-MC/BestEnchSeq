#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>
#include "core.h"

class Algorithm : public QObject
{
    Q_OBJECT
public:
    explicit Algorithm(QObject *parent = nullptr);
    virtual ~Algorithm() {}

    void start(QSet<int> es, _Item tar, _ItemPool ip, MCE mce, ALGCFG cfg); // 开始计算（槽函数）

    static ulong levelToPoint(int lvl);                 // 通用计算：等级值转经验值
    static int mergeLevel(int ench_id, int l1, int l2); // 通用计算：魔咒等级合并

protected:
    virtual void run() = 0; // 算法

    int findEnch(const _Item &item, const _Ench &ench, bool strict = false); // 在物品中查找魔咒
    QList<QPoint> findItemWith(int ench_id);                                 // 在物品池中查找所有指定魔咒ID的魔咒位置

    uint8_t checkProcessable(const _Item &item, const _Ench &ench);           // 检查魔咒是否可以合并到物品中（0x0: Pass; 0x1: Failed; 0x2: Pass but has confl.）
    bool checkConfig(const _ItemPool &ip, QList<int> *details = nullptr);     // 检查物品配置是否符合基本要求
    int expCost(const _Item &a, const _Item &b, MCE mce, uint8_t mode = 0x2); // 计算物品合并花费
    _Item forge(const _Item &a, const _Item &b);                              // 合并物品

    QSet<int> ench_set;  // 物品所在分组所包括的魔咒的列表
    _Item target_item;   // 目标物品
    _ItemPool item_pool; // 物品池
    MCE edition;         // MC编译版本
    ALGCFG alg_config;   // 算法配置

private:
signals:
    void stepForwarded(uint current, uint total); // 计算进度报告
    void finished(Summary s, _FlowStack fs);      // 计算完成信号
};

class FinTriangle
{
public:
    void update(int n);          // 更新列表
    int vaule(int x);            // 取值
    QList<int> arcValues(int x); // 反函数（多值）

private:
    QList<int> data; // 数据
};

#endif // ALGORITHM_H
