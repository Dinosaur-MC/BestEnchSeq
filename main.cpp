#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("WindowsVista");
    int ret = -1;
    do
    {
        MainWindow w;
        w.show();
        ret = a.exec();
    }while(ret == 5201314);
    return ret;
}
