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

    static int findEnch(const _Item &item, const _Ench &ench, bool strict = false);                 // 在物品中查找魔咒
    static int findEnch(const _EnchList &enchs, const _Ench &ench, bool strict = false);         // 在魔咒列表中查找魔咒
    static QList<QPoint> findItemWith(int ench_id, const _ItemPool &ip);                            // 在物品池中查找所有指定魔咒ID的魔咒位置
    static bool checkConfig(const _Item &item, const _ItemPool &ip, QList<int> *missing = nullptr); // 检查物品配置是否符合基本要求

    QString name() { return algorithm_name; }
    QString version() { return algorithm_version; }
    QString author() { return algorithm_author; }

protected:
    void setName(QString name) { algorithm_name = name; }
    void setVersion(QString version) { algorithm_version = version; }
    void setAuthor(QString author) { algorithm_author = author; }

    virtual void run() = 0; // 算法

    uint8_t checkProcessable(const _Item &item, const _Ench &ench);           // 检查魔咒是否可以合并到物品中（0x0: Pass; 0x1: Failed; 0x2: Pass but has confl.）
    int expCost(const _Item &a, const _Item &b, MCE mce, uint8_t mode = 0x2); // 计算物品合并花费
    _Item forge(const _Item &a, const _Item &b);                              // 合并物品

    QSet<int> ench_set;  // 物品所在分组所包括的魔咒的列表
    _Item target_item;   // 目标物品
    _ItemPool item_pool; // 物品池
    MCE edition;         // MC编译版本
    ALGCFG alg_config;   // 算法配置

    QString algorithm_name;
    QString algorithm_version;
    QString algorithm_author;

signals:
    void stepForwarded(uint current, uint total); // 计算进度报告
    void finished(Summary s, _FlowStack fs);      // 计算完成信号
};

QT_BEGIN_NAMESPACE
#define AlgorithmInterface_ID "AlgorithmInterface_ID"
Q_DECLARE_INTERFACE(Algorithm, AlgorithmInterface_ID);
QT_END_NAMESPACE

#endif // ALGORITHM_H
