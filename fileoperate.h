#ifndef FILEOPERATE_H
#define FILEOPERATE_H

#include <QObject>
#include "datamanager.h"

class FileOperate : public QObject
{
    Q_OBJECT
public:
    explicit FileOperate(QObject *parent = nullptr);

    void saveConfig();  //保存配置
    void saveWeapon();  //保存武器（用于快速自定义武器功能）
    void saveEnchantmentTable();    //保存魔咒（用于快速自定义魔咒功能）

    void loadConfig();  //加载配置
    void loadWeapon();  //加载武器
    void loadEnchantmentTable();    //加载魔咒

    void saveExport(QString dir_str = "./exports/");  //保存输出结果

signals:

};

#endif // FILEOPERATE_H
