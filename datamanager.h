/*
 * 【数据管理器】
 * 定义全局变量
 * 用于存储程序全局数据
 * 方便各模块读写全局数据
*/

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "base.h"
#include "itempool.h"

//定义快捷访问宏
#define DM DataManager::Instance()

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);
    static DataManager* Instance(); //获取全局单例类指针

    //Function
    void reinit();  //变量重置，用于程序启动和重启时
    void uploadData(Config);   //上传数据
    void uploadData(EnchTable*, int len);
    void uploadData(Weapon*, int len);
    void upload(Ench*, int len, int p);    //上传内容
    void upload(ItemPool*);
    void upload(Step*, int len);
    static Ench *resizeOriginEnchList(int size);
    static Ench *resizeNeededEnchList(int size);
    static Step *resizeFlowList(int size);

    //Config
    Config config; //存储配置信息
    MCE edition;    //存储模式信息：MC编译版本
    ICM itemconfig; //存储模式信息：物品配置模式
    ALGM alg_mode;    //存储模式信息：算法
    bool addition[3];  //存储模式信息：附加条件

    //Array
    int ench_table_l;   //定义记录数组长度的整型变量，下同
    int weapon_l;
    int origin_ench_l;
    int needed_ench_l;
//    int available_item_l;
    int flow_list_l;
    EnchTable *ench_table;  //定义魔咒池，存储所有魔咒的全部相关信息
    Weapon *weapon; //定义武器池（可锻造附魔的物品的统称，包括但不限于武器、盔甲和工具），存储所有武器的基本信息
    Ench *origin_ench;  //定义魔咒列表，为武器初始带有的魔咒
    Ench *needed_ench;  //定义魔咒列表，为需要达成的魔咒
//    Item *available_item;   //定义物品池，为用于合并的物品或魔咒
    Step *flow_list;    //定义步骤列表，存储附魔流程信息

    //Variable
    Item *OriginItem;   //定义原始（输入）物品，即目标武器
    Item *OutputItem;   //定义输出物品，即锻造完成后输出的物品（武器）
    ItemPool *item_pool;    //定义物品池，存储物品
    int maxLevelCost;
    int sumLevelCost;    //定义整型变量，总等级花费
    int sumPointCost;   //定义整型变量，总经验值花费
    int costTime;   //计算用时

//    Summary sum;

    //Other
    bool isFirstLaunch;    //软件是否为首次启动（若启动时找不到配置文件则为真），用于启动时展示向导和声明等操作
    bool isUpdated; //软件是否已更新，用于启动时展示更新完成提示

signals:

};

#endif // DATAMANAGER_H
