#ifndef SUMMARY_H
#define SUMMARY_H

#include <QObject>
#include "base/Item.h"
#include "base/Config.h"

struct Summary {
    MCE edition;
    ALGM algorithm;
    Item inputItem;
    Item outputItem;
    int costLevel;
    int costPoint;
    int stepCount;
    int costTime;
    bool isProcessable;
};

#endif // SUMMARY_H
