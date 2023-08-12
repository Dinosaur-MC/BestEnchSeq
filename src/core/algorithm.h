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

    void start(_Group g, _Item tar, _ItemPool ip, MCE mce, ALGCFG cfg); // 开始计算（槽函数）

    static ulong levelToPoint(int lvl);                 // 通用计算：等级值转经验值
    static int mergeLevel(int ench_id, int l1, int l2); // 通用计算：魔咒等级合并

    static int findEnch(const _Item &item, const _Ench &ench, bool strict = false);                 // 在物品中查找魔咒
    static int findEnch(const _EnchList &enchs, const _Ench &ench, bool strict = false);            // 在魔咒列表中查找魔咒
    static QList<QPoint> findItemWith(int ench_id, const _ItemPool &ip);                            // 在物品池中查找所有指定魔咒ID的魔咒位置
    static bool checkConfig(const _Item &item, const _ItemPool &ip, QList<int> *missing = nullptr); // 检查物品配置是否符合基本要求

    QString name() { return algorithm_name; }
    QString version() { return algorithm_version; }
    QString author() { return algorithm_author; }

protected:
    void setName(QString name) { algorithm_name = name; }
    void setVersion(QString version) { algorithm_version = version; }
    void setAuthor(QString author) { algorithm_author = author; }

    virtual void run() = 0; // 运行算法

    uint8_t checkProcessable(const _Item &item, const _Ench &ench);     // 检查魔咒是否可以合并到物品中（0x00: Pass; 0x01: Failed; 0x02: Pass but has confl.）
    int expCost(const _Item &a, const _Item &b, uint8_t mode = 0x0F);   // 计算物品的合并花费
    int expCost(const _Item &it, bool mode = true);                     // 预算物品的合并花费
    int penaltyCost(int n);                                             // 计算惩罚花费
    _Item forge(const _Item &a, const _Item &b);                        // 合并物品

    MCE edition;                // MC编译版本
    _Group group;               // 物品所在分组所包括的魔咒的列表
    _Item target_item;          // 目标物品
    _ItemPool item_pool;        // 物品池

    ALGCFG alg_config;      // 算法配置
    _FlowStack flow_stack;  // 流程集

    QString algorithm_name;
    QString algorithm_version;
    QString algorithm_author;

signals:
    void stepForwarded(uint current, uint total); // 计算进度报告
    void finished(_FlowStack fs, _ItemPool unused);                 // 计算完成信号
};

QT_BEGIN_NAMESPACE
#define AlgorithmInterface_ID "AlgorithmInterface_ID"
Q_DECLARE_INTERFACE(Algorithm, AlgorithmInterface_ID);
QT_END_NAMESPACE

#endif // ALGORITHM_H
