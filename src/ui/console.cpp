#include "console.h"
#include "windows.h"
#include <QApplication>

Console::Console(QObject *parent)
    : QObject{parent}
{

}

int Console::run()
{
    qDebug() << "Console running";

    // 创建并初始化控制台
    AllocConsole();
    system("chcp 65001");

    // 重定向数据流
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);

    int ret = 0;
    while(1)
    {
        /* Functions */
    }

    // 结束
    system("pause");
    fclose(stdout);
    fclose(stdin);
    FreeConsole();

    qDebug() << "Console exited";
    return ret;
}

int Console::exit(int code)
{
    return code;
}
