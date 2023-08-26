#include "algmanager.h"
#include "algorithms/algenumerate.h"
#include "algorithms/alggreedy.h"
#include "algorithms/alghuffmantree.h"
#include <QDir>
#include <QFile>

AlgManager alg_manager;

AlgManager::AlgManager()
{
}

AlgManager::~AlgManager()
{
}

QList<AlgorithmInfo> AlgManager::getInternalAlgorithm()
{
    QList<AlgorithmInfo> alg_list;
    AlgHuffmanTree alg_1;
    AlgGreedy alg_2;
    AlgEnumerate alg_3;
    alg_list.append({alg_1.name(), alg_1.version(), alg_1.supported_cfg(), alg_1.author(), alg_1.link()});
    alg_list.append({alg_2.name(), alg_2.version(), alg_2.supported_cfg(), alg_2.author(), alg_2.link()});
    alg_list.append({alg_3.name(), alg_3.version(), alg_3.supported_cfg(), alg_3.author(), alg_3.link()});
    return alg_list;
}

QList<AlgorithmInfo> AlgManager::detectAlgorithm()
{
    algorithms.clear();

    QDir dir(PATH_ALGORITHMS);
    if (!dir.exists())
    {
        dir.mkdir(PATH_ALGORITHMS);
        return QList<AlgorithmInfo>();
    }

    QStringList file_list = dir.entryList({"*.dll"}, QDir::Files);
    if (!file_list.isEmpty())
    {
        QList<AlgorithmInfo> info_list;
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
                info.supported_cfg = ptr->supported_cfg();
                info.author = ptr->author();
                info.link = ptr->link();
                info_list.append(info);
                algorithms.insert(file, info);
                loader.unload();
            }
        }
        return info_list;
    }

    return QList<AlgorithmInfo>();
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
    if (isLoaded())
        loader.unload();
}

bool AlgManager::isLoaded()
{
    return loader.isLoaded();
}
