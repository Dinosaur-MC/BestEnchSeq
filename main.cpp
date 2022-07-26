#include <QCoreApplication>
#include <iostream>
#include "windows.h"

#include <QApplication>
#include "awindow.h"

#include "basicdata.h"

using namespace std;


int Analyze(QStringList);


int main(int argc, char *argv[])
{
    if(argc > 1)    // 检查参数个数，若超过1个则不启动GUI
    {
        QStringList str;
        for(int i = 0; i < argc; i++)
            str.append(argv[i]);

        if(!AttachConsole(ATTACH_PARENT_PROCESS))   // 捕获或启动终端
            AllocConsole();
        freopen("CONOUT$","w", stdout); // 重定向数据流
        freopen("CONOUT$","w", stderr);
        freopen("CONIN$","r", stdin);

        int mode = Analyze(str);    // 参数分析
        int ret = 0;
        if(mode)
        {
            QCoreApplication app(argc, argv);   // 创建控制台程序

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
        QApplication a(argc, argv); // 创建GUI程序
        int ret = 0;
        do {
            AWindow w;
            w.show();
            ret = a.exec();
        } while(ret == RESTART_CODE);
        return ret;
    }
}


int Analyze(QStringList str)
{
    if(str.count() > 2) // 限定参数个数为2 （实际使用为1，第一个参数为程序文件名）
    {
        cout << endl << "Error: Too many arguments" << endl;
        return 0;
    }

    if(str.at(1) == "-h" || str.at(1) == "--help")  // 获取帮助
    {
        cout << endl << "-h, --help : Call out this message." << endl;
        cout << "-v, --version : Show the application version." << endl;
        cout << "-c, --console : Launch with console mode, else with no argument will launch with GUI mode." << endl;
    }
    else if(str.at(1) == "-v" || str.at(1) == "--version")  // 输出程序版本及版本ID
    {
        cout << endl << PROGRAM_NAME_EN << " Version: " << VERSION << "(" << VERSION_ID << ")" << endl;
    }
    else if(str.at(1) == "-c" || str.at(1) == "--console")  // 以控制台模式启动程序
    {
        cout << endl << str.at(0).toUtf8().data() << ": Hello Qt" << endl;
        return 1;
    }
    else    // 处理无效参数
        cout << endl << "Error: Unknown argument" << endl;

    return 0;
}
