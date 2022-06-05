#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include "base.h"

#define DM DataManager::Instance()

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);

    static DataManager* Instance(); // 获取全局单例类指针
    void reset();   // 重置数据

    Config config;
    bool isFirstLaunch;
    bool isUpdated;

};

#endif // DATAMANAGER_H
