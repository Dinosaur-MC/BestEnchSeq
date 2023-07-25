#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include <QObject>
#include "core.h"
#include <QDir>
#include <QFile>


struct raw_Weapon {
    QString name;   // 武器名
    QIcon icon; // 武器图标
};

struct raw_EnchPlus {
    QString name;   // 魔咒名称
    MCE edition;    // 魔咒适用版本
    int max_level;  // 魔咒最高等级
    int poor_max_level; // 30级附魔台附魔一般最高可获得等级及宝藏魔咒（实际上用于BasicEBook物品配置中，是魔咒的最大可直接获得的魔咒等级，可自定义）
    int multiplier[2];  // 魔咒乘数，[0]为物品乘数，[1]为附魔书乘数
    QVector<QString> repulsion; // 冲突的魔咒
    QVector<bool> suitable;  // 适配的武器
};


class FileOperator : public QObject
{
    Q_OBJECT
public:
    explicit FileOperator(Core *c, QObject *parent = nullptr);

    // 加载、导入数据
    void loadConfig();  // *需添加准确的数据有效性检验
    void loadWeapons();
    void loadEnchantments();
    void loadFlow();

    // 保存、导出数据
    void saveConfig();
    void saveWeapons();
    void saveEnchantments();
    void saveFlow();

    // 信息转换
    void updateToCore();
    void updateToLocal(bool protect = false);

private:
    Core *core; // 数据流目标

    // 源数据
    QVector<raw_Weapon> rwps;
    QVector<raw_EnchPlus> reps;

    MDebug mdb; // Debug Msg

signals:

};

// 文件读写
bool readFile(QFile *file, QString *str, QString default_data = "");
bool writeFile(QFile *file, QString str);
bool writeFile(QFile *file, QStringList strs);

// 数据转换 <Type> -> QString   // 降维打击
QString stringFoil(QStringList strs);
QString stringFoil(Config cfg);
QString stringFoil(QVector<raw_EnchPlus> reps);
QString stringFoil(QVector<raw_Weapon> rwps);
QString stringFoil(Summary summary, QVector<FlowStep> fs, QString addition);

#endif // FILEOPERATOR_H
