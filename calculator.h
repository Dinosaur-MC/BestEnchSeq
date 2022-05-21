/*
 * [计算模块]
 * 使用QThread
 * 在子线程进行计算
 * 与等待窗口[WaitWidget]配合
*/

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QThread>
#include "datamanager.h"
#include "itempool.h"

class Calculator : public QThread
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);

    void preparation(); //预处理
    void run();   //开始计算

    bool checkAvailability();   //检查列表可行性，用于高级物品配置
    unsigned long long evaluateComplexity();  //复杂度预测

    void Alg_GlobeAverage();    //核心算法
    void Alg_DifficultyFirst();
    void Alg_Greedy();
    void Alg_Enumeration();
    void Alg_SimpleEnumeration();

    void uploadData();  //数据上传
    bool isFinished;

private:
    int flow_step;  //步骤数
    int flow_l; //流程大小
    Step *flow; //存储流程
    ForgeMode additional_mode;  //附加模式
    ItemPool pool;  //物品池
    unsigned long long complexity;  //枚举计算复杂度

    bool isPassed;   //是否通过物品检查
    int costTime;

signals:
    void isDone();  //计算结束信号

};

#endif // CALCULATOR_H
