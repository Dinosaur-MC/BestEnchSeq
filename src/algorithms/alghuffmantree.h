#ifndef ALGHUFFMANTREE_H
#define ALGHUFFMANTREE_H

#include <QObject>
#include "core/algorithm.h"

class AlgHuffmanTree : public Algorithm
{
    Q_OBJECT
public:
    explicit AlgHuffmanTree(QObject *parent = nullptr);

private:
    virtual void run() override;

signals:
};

class FinTriangle // 重复频次数列模型
{
public:
    void update(int n);          // 更新列表
    int vaule(int x);            // 取值
    QList<int> arcValues(int x); // 反函数（多值）

private:
    QList<int> data; // 数据
};

#endif // ALGHUFFMANTREE_H
