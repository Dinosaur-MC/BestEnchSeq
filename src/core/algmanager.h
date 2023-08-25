#ifndef ALGMANAGER_H
#define ALGMANAGER_H

#include <QPluginLoader>
#include "algorithm.h"

struct AlgorithmInfo
{
    QString name;
    QString version;
    ALGCFG supported_cfg;
    QString author;
    QString link;
};

class AlgManager
{
public:
    explicit AlgManager();
    ~AlgManager();

    QList<AlgorithmInfo> getInternalAlgorithm();
    QList<AlgorithmInfo> detectAlgorithm();

    Algorithm *loadAlgorithm(QString alg_fname);
    void unloadAlgorithm();
    bool isLoaded();

private:
    QMap<QString, AlgorithmInfo> algorithms;
    QPluginLoader loader;
};

extern AlgManager alg_manager;

#endif // ALGMANAGER_H
