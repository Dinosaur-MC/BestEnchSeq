#include "windows.h"
#include <QCoreApplication>
#include <iostream>

#include <QApplication>
#include "awindow.h"

#include "basicdata.h"

using namespace std;


int Analyze(QStringList);


int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        QStringList str;
        for(int i = 0; i < argc; i++)
            str.append(argv[i]);

        if(!AttachConsole(ATTACH_PARENT_PROCESS))
            AllocConsole();
        freopen("CONOUT$","w", stdout);
        freopen("CONOUT$","w", stderr);
        freopen("CONIN$","r", stdin);

        int mode = Analyze(str);
        int ret = 0;
        if(mode)
        {
            QCoreApplication app(argc, argv);

            /* Console Version Class */

            system("pause");
        }
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


int Analyze(QStringList str)
{
    if(str.count() > 2)
    {
        cout << endl << "Error: Too many arguments" << endl;
        return 0;
    }

    if(str.at(1) == "-h" || str.at(1) == "--help")
    {
        cout << endl << "-h, --help : Call out this message." << endl;
        cout << "-v, --version : Show the application version." << endl;
        cout << "-c, --console : Launch with console mode, else with no argument will launch with GUI mode." << endl;
    }
    else if(str.at(1) == "-v" || str.at(1) == "--version")
    {
        cout << endl << PROGRAM_NAME_EN << " Version: " << VERSION << "(" << VERSION_ID << ")" << endl;
    }
    else if(str.at(1) == "-c" || str.at(1) == "--console")
    {
        cout << endl << str.at(0).toUtf8().data() << ": Hello Qt" << endl;
        return 1;
    }
    else
        cout << endl << "Error: Unknown argument" << endl;

    return 0;
}
