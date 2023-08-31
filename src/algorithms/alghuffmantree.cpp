#include "alghuffmantree.h"

AlgHuffmanTree::AlgHuffmanTree(QObject *parent)
    : Algorithm{parent}
{
    algorithm_name = tr("Huffman Tree");
    algorithm_version = "2.0";
    supported_configuration = (ALGCFG)0x1B;
    algorithm_author = "Dinosaur-MC";
    algorithm_link = "https://github.com/Dinosaur-MC/BestEnchSeq/tree/dev/src/algorithms";
}

void FinTriangle::update(int n)
{
    data.clear();
    for (int i = 0; i < n; i++)
    {
        int n = 0;
        for (int j = i; j > 0; j /= 2)
        {
            if (j % 2 == 1)
                n++;
        }
        data.append(n);
    }
}
int FinTriangle::vaule(int x)
{
    if (x < data.size())
        return data.at(x);
    else
        return -1;
}
QList<int> FinTriangle::arcValues(int x)
{
    QList<int> values;
    for (int i = 0; i < data.size(); i++)
    {
        if (data.at(i) == x)
            values.append(i);
    }
    return values;
}

void AlgHuffmanTree::run()
{
}
