#ifndef SUMMARY_H
#define SUMMARY_H

#include <QObject>
#include "base/Item.h"

enum MCE {All=1, Java, Bedrock};  //枚举变量，MC编译版本
enum ICM {AllLevelEBook=1, BasicEBook, AdvanceMode};    //枚举变量，物品配置模式
enum ALGM {GlobalAverage=1, DifficultyFirst, Greedy, Enumeration, SimpleEnumeration};   //枚举变量，算法名

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
