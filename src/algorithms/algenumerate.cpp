#include "algenumerate.h"

AlgEnumerate::AlgEnumerate(QObject *parent)
    : Algorithm{parent}
{
    algorithm_name = tr("Enumerate");
    algorithm_version = "0.0";
    supported_configuration = (ALGCFG)0x1F;
    algorithm_author = "Dinosaur-MC";
    algorithm_link = "https://github.com/Dinosaur-MC/BestEnchSeq/tree/dev/src/algorithms";
}

void AlgEnumerate::run()
{

}
