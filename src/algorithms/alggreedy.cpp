#include "alggreedy.h"

AlgGreedy::AlgGreedy(QObject *parent)
    : Algorithm{parent}
{
    algorithm_name = tr("Greedy");
    algorithm_version = "1.0";
    supported_configuration = (ALGCFG)0x1F;
    algorithm_author = "Dinosaur-MC";
    algorithm_link = "https://github.com/Dinosaur-MC/BestEnchSeq/tree/dev/src/algorithms";
}

void AlgGreedy::run()
{

}
