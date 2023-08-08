#include "algmanager.h"
#include <QDir>
#include <QFile>

AlgManager alg_manager;

AlgManager::AlgManager()
{
}

AlgManager::~AlgManager()
{
}

void AlgManager::detectAlgorithm()
{
    algorithms.clear();

    QDir dir("./algorithms");
    if (!dir.exists())
    {
        dir.mkdir("./algorithms");
        return;
    }

    QStringList file_list = dir.entryList({"*.dll"}, QDir::Files);
    if (!file_list.isEmpty())
    {
        foreach (auto file, file_list)
        {
            loader.setFileName(file);
            if (loader.load())
            {
                Algorithm *ptr = (Algorithm *)loader.instance();
                if (ptr == nullptr)
                    continue;

                AlgorithmInfo info;
                info.name = ptr->name();
                info.version = ptr->version();
                info.author = ptr->author();
                algorithms.insert(file, info);
                loader.unload();
            }
        }
    }
}

Algorithm *AlgManager::loadAlgorithm(QString alg_fname)
{
    if (algorithms.contains(alg_fname))
    {
        unloadAlgorithm();
        loader.setFileName(alg_fname);
        if (loader.load())
            return (Algorithm *)loader.instance();
    }
    return nullptr;
}

void AlgManager::unloadAlgorithm()
{
    if (loader.isLoaded())
        loader.unload();
}

bool AlgManager::isLoaded()
{
    return loader.isLoaded();
}
