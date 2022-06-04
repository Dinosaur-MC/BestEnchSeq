#include <QApplication>
#include "AWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int ret = -1;
    while(1)
    {
        AWindow w;
        w.show();
        ret = a.exec();
        if(ret != RESTART_CODE)
            break;
    }
    return ret;
}
