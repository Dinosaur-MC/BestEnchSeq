#include <QApplication>
#include <QCommandLineParser>
#include <QFontDatabase>
#include <iostream>
#include "launcher.h"
#include "windows.h"

using namespace std;


int main(int argc, char *argv[])
{
    if(argc > 1)    // 检查参数个数，若超过1个则不启动GUI
    {
        QCoreApplication app(argc, argv);   // 创建无GUI的应用程序实例

        // 定义命令解析实例，并设置其属性
        QCommandLineParser parser;
        parser.setApplicationDescription("BestEnchSeq made by Dinosaur_MC");
        parser.addHelpOption();
        parser.addVersionOption();

        // 定义命令
        QCommandLineOption console(QStringList() << "c" << "console", "以控制台模式启动");
        parser.addOption(console);
        QCommandLineOption test(QStringList() << "t" << "test", "Test Option");
        parser.addOption(test);


        parser.process(app);

        // 命令实现
        if(parser.isSet(console))   // 控制台模式
        {
            Launcher lunc(CORE, &app);
            return lunc.launch();
        }
        else
        {
            if(!AttachConsole(ATTACH_PARENT_PROCESS)) // 捕获或创建控制台
                AllocConsole();

            // 重定向数据流
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
            freopen("CONIN$", "r", stdin);
        }

        if(parser.isSet(test))  // 测试命令
        {
            cout << "Hello World" << endl;
        }


        // End
        fclose(stdout);
        fclose(stdin);
        FreeConsole();
        return 0;
    }
    else
    {
        QApplication app(argc, argv); // 创建带GUI的应用程序实例

        int fontId = QFontDatabase::addApplicationFont(":/font/HarmonyOS_Sans_SC_Regular.ttf");    // 加载字体文件
        if (fontId == -1)
        {
            qDebug() << "Load font failed!";
        }
        else
        {
            QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);    // 获取字体名称
            if (fontName != nullptr)
                app.setFont(fontName);
        }

        Launcher lunc(CORE, &app);
        return lunc.launch();
    }
}

