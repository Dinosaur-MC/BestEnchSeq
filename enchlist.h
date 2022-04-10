#ifndef ENCHLIST_H
#define ENCHLIST_H

#include <QListWidget>
#include "datamanager.h"
#include "enchlistwidget.h"

class EnchList : public QListWidget
{
    Q_OBJECT
public:
    explicit EnchList(QWidget *parent = nullptr);

    void setOutputAddress(Ench*, Ench* (*func)(int) = nullptr); //设置上传输出地址
    void setPlate(Ench *p, int len);    //设置底片
    bool setWeapon(QString name = "");    //设置武器并加载配置
    QString Weapon();   //获取当前配置的武器名称
    int enchCount();    //获取魔咒数量
    void refresh(); //刷新
    void uploadList();  //上传数据至数据管理器
    void getAvailable();    //获取可用魔咒
    void checkRepulsion();  //检查冲突魔咒并处理

    bool isChanged();   //是否变更过

private:
    int plate_l;
    Ench *plate; //基础魔咒列表，若不为空，则以此为基础配置魔咒
    int current_weapon_index;

    Ench* (*resize)(int);
    int output_l;
    Ench *output;

    int available_ench_l;
    EnchPlus *available_ench;

    int changing;

signals:
    void enchChanged(); //魔咒已更改

};

#endif // ENCHLIST_H
