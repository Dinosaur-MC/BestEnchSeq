#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include "basic.h"
#include "itempool.h"

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);

    void Alg_DifficultyFirst();
    void Alg_Hamming();
    void Alg_Enumeration();

    void uploadData();

private:
    void sortPool(QVector<Item> *its, int left, int right)
    {
        if(left > right)
            return;

        int temp = ItemPool::preForge(its->at(left), Normal);
        int i = left;
        int j = right;

        while(i != j)
        {
            while(ItemPool::preForge(its->at(j), Normal) <= temp && j > i)
                j--;
            while(ItemPool::preForge(its->at(i), Normal) >= temp && j > i)
                i++;
            if(i < j)
                its->swapItemsAt(i, j);
        }
        its->swapItemsAt(left, i);

        sortPool(its, left, i-1);
        sortPool(its, i+1, right);
    }
    int duplicationSeries(int x)    // 右值频次数列
    {
        // 定义域和值域都是自然数集（ x,y∈ N）

        if(x < 0)   // 界定定义域为自然数，超出范围则返回错误代码 -1
            return -1;

        int y = 0;
        for(int j = x; j > 0; j/=2) // 计算右值频次
        {
            if(j%2 == 1)
                y++;
        }

        return y;
    }

    QVector<int> dupFloorMembers(int x, int n)
    {
        QVector<int> y;
        for(int i = 0; i < n; i++)
        {
            if(x == duplicationSeries(i))
                y.append(i);
        }
        return y;
    }

    int flow_l, flow_step;
    Step *flow;
    ForgeMode additional_mode;
    ItemPool pool;

signals:

};

#endif // CALCULATOR_H
