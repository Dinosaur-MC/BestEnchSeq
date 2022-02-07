#include "best_ench_seq.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int ret = -1;
    do
    {
        BestEnchSeq w;
        w.show();
        ret = a.exec();
    }while(ret == 1314);
    return ret;
}
