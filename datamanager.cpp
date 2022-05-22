#include "datamanager.h"
Q_GLOBAL_STATIC(DataManager, dm);   //创建全局单例类，存储全局数据


DataManager::DataManager(QObject *parent)
    : QObject{parent}
{
    qDebug() << "[DataManager]";
    reinit();
    qDebug() << "- DataManager";
}

DataManager* DataManager::Instance()
{
    return dm;
}

void DataManager::reinit()
{
    delete [] ench_table;
    delete [] weapon;
    delete [] origin_ench;
    delete [] needed_ench;
//    delete [] available_item;
    delete [] flow_list;
    delete OriginItem;
    delete OutputItem;
    delete item_pool;

    config = {FILEVERSION, MCE::Java, ICM::AllLevelEBook, ALGM::GlobalAverage, "", 0, 0, 0, 1, 0};
    edition = MCE::Java;
    itemconfig = ICM::AllLevelEBook;
    alg_mode = ALGM::GlobalAverage;
    memset(addition, 0, sizeof(addition));

    ench_table_l = INIT_LENGTH;
    weapon_l = INIT_LENGTH;
    origin_ench_l = INIT_LENGTH;
    needed_ench_l = INIT_LENGTH;
//    available_item_l = INIT_LENGTH;
    flow_list_l = INIT_LENGTH;
    ench_table = new EnchTable[ench_table_l];
    weapon = new Weapon[weapon_l];
    origin_ench = new Ench[origin_ench_l];
    needed_ench = new Ench[needed_ench_l];
//    available_item = new Item[available_item_l];
    flow_list = new Step[flow_list_l];

    OriginItem = new Item();
    OutputItem = new Item();
    item_pool = new ItemPool();
    sumLevelCost = 0;
    sumPointCost = 0;

//    sum = {};

    isFirstLaunch = false;
    isUpdated = false;
    updateAvailablity = false;
    calcProgress = 0;
}

void DataManager::uploadData(Config c)
{
    config.config_version = c.config_version;
    config.default_edition = c.default_edition;
    config.default_algorithm = c.default_algorithm;
    config.export_path = c.export_path;
    config.constant = c.constant;
    config.enableCustomEn = c.enableCustomEn;
    config.enableCustomWe = c.enableCustomWe;
    config.autoCheckUpdate = c.autoCheckUpdate;
    config.deverloperMode = c.deverloperMode;
}

void DataManager::uploadData(EnchTable* et, int len)
{
    if(len < 1)
        return;
    delete [] ench_table;
    ench_table = new EnchTable[len];
    ench_table_l = len;

    for(int i = 0; i < len; i++)
        ench_table[i] = et[i];
}

void DataManager::uploadData(Weapon* w, int len)
{
    if(len < 1)
        return;
    delete [] weapon;
    weapon = new Weapon[len];
    weapon_l = len;

    for(int i = 0; i < len; i++)
        weapon[i] = w[i];
}

void DataManager::upload(Ench* e, int len, int p)
{
    if(len < 1)
        return;
    if(p == 0)
    {
        delete [] origin_ench;
        origin_ench = new Ench[len];
        origin_ench_l = len;

        for(int i = 0; i < len; i++)
            origin_ench[i] = e[i];
    }
    else if (p == 1)
    {
        delete [] needed_ench;
        needed_ench = new Ench[len];
        needed_ench_l = len;

        for(int i = 0; i < len; i++)
            needed_ench[i] = e[i];
    }
    else
        return;
}

void DataManager::upload(ItemPool *ip)
{
    item_pool->cloneFrom(ip);
}

void DataManager::upload(Step* s, int len)
{
    flow_list_l = len;
    maxLevelCost = 0;
    sumLevelCost = 0;
    sumPointCost = 0;
    if(len < 1)
        return;
    delete [] flow_list;
    flow_list = new Step[len];

    for(int i = 0; i < len; i++)
    {
        flow_list[i] = s[i];
        maxLevelCost = std::max(maxLevelCost, s[i].cost);
        sumLevelCost += s[i].cost;
        if(s[i].cost <= 16)
            sumPointCost += (s[i].cost * s[i].cost + 6 * s[i].cost);
        else if(s[i].cost <= 31)
            sumPointCost += (2.5 * s[i].cost * s[i].cost - 40.5 * s[i].cost + 360);
        else
            sumPointCost += (4.5 * s[i].cost * s[i].cost - 162.5 * s[i].cost + 2220);
    }
}

Ench *DataManager::resizeOriginEnchList(int size)
{
    delete [] DM->origin_ench;
    DM->origin_ench = new Ench[size];
    DM->origin_ench_l = size;
    return DM->origin_ench;
}

Ench *DataManager::resizeNeededEnchList(int size)
{
    delete [] DM->needed_ench;
    DM->needed_ench = new Ench[size];
    DM->needed_ench_l = size;
    return DM->needed_ench;
}

Step *DataManager::resizeFlowList(int size)
{
    delete [] DM->flow_list;
    DM->flow_list = new Step[size];
    DM->flow_list_l = size;
    return DM->flow_list;
}

