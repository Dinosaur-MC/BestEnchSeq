#ifndef ALGMANAGER_H
#define ALGMANAGER_H

#include <QPluginLoader>
#include "algorithm.h"

struct AlgorithmInfo
{
    QString name;
    QString version;
    QString author;
};

class AlgManager
{
public:
    explicit AlgManager();
    ~AlgManager();

    void detectAlgorithm();

    Algorithm *loadAlgorithm(QString alg_fname);
    void unloadAlgorithm();
    bool isLoaded();

private:
    QMap<QString, AlgorithmInfo> algorithms;
    QPluginLoader loader;
};

extern AlgManager alg_manager;

#endif // ALGMANAGER_H
