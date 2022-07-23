#include "windows.h"
#include <QCoreApplication>
#include <QDebug>
#include <iostream>

#include <QApplication>
#include "awindow.h"

#include "basicdata.h"



int Analyze(QStringList);


int main(int argc, char *argv[])
{
//    if(AttachConsole(ATTACH_PARENT_PROCESS))
    if(argc > 1)
    {
        AllocConsole();
        freopen("CONOUT$","w", stdout);
        freopen("CONOUT$","w", stderr);
        freopen("CONIN$","r", stdin);
        QCoreApplication app(argc, argv);
        int ret = 0;

        std::cout << "Hello Qt" << std::endl;

//        ret = app.exec();
        system("pause");
        fclose(stdout);
        fclose(stdin);
        FreeConsole();
        return ret;
    }
    else
    {
        QApplication a(argc, argv);
        int ret = 0;
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
}

