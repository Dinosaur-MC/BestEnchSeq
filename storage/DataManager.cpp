#include "storage/DataManager.h"

Q_GLOBAL_STATIC(DataManager, dm);

DataManager::DataManager(QObject *parent)
    : QObject{parent}
{
    reset();
}


DataManager *DataManager::Instance()    //获取全局单例类指针
{
    return dm;
}

void DataManager::reset()
{
    config = {};
    isFirstLaunch = false;
    isUpdated = false;
}
