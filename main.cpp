#include <QApplication>
#include "awindow.h"
#include "base.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DM;
    int ret = -1;
    while(1)
    {
        AWindow w;
        w.show();
        ret = a.exec();
        if(ret == RESTART_CODE)
            DM->reinit();
        else
            break;
    }
    return ret;
}
