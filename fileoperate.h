#ifndef FILEOPERATE_H
#define FILEOPERATE_H

#include <QObject>
#include "datamanager.h"

#define FILEHEAD "\xef\xbb\xbf\x0d\x0a"

class FileOperate : public QObject
{
    Q_OBJECT
public:
    explicit FileOperate(QObject *parent = nullptr);

    static void saveConfig();  //保存配置
    static void saveWeapon();  //保存武器（用于快速自定义武器功能）
    static void saveEnchantmentTable();    //保存魔咒（用于快速自定义魔咒功能）

    static void loadConfig();  //加载配置
    static void loadWeapon();  //加载武器
    static void loadEnchantmentTable();    //加载魔咒

    static void saveExport(QString dir_str = "./exports/");  //保存输出结果

signals:

};

#endif // FILEOPERATE_H
