#define BUILD_GUI 1
#include "core/core.h"
#include "core/fileoperator.h"

#if BUILD_GUI == 0
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        // 创建应用 #1
        QCoreApplication app(argc, argv);
        QCoreApplication::setOrganizationName("DMC Studio");
        QCoreApplication::setOrganizationDomain(LINK_HOME_PAGE);
        QCoreApplication::setApplicationName(TEXT_PROGRAM_NAME_CN);

        // 加载应用配置 #2
        registerSettings();
        if (!QFile::exists(FILE_CONFIG))
        {
            // + First-launch Notice
        }
        if (!FileOperator::loadConfig(FILE_CONFIG) || global_settings["version/file_version"].toInt() < FILEVERSION)
        {
            if (!FileOperator::saveConfig(FILE_CONFIG))
            {
                return 3;
            }
            // + Upgrade Notice
        }

        // 清理内存数据 #3
        initializeGlobalTable();

        // 加载 Table #4
        if (!FileOperator::loadTableData("default_table.json"))
        {
            if (!FileOperator::loadTableData("default_table.csv"))
                return 5;
        }
        global_p_ench_table = convertEnchTable(global_u_ench_table);

        // 启动 Console 程序

        app.exec();
    }
    return 0;
}

#endif

#if BUILD_GUI == 1
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    while (1)
    {
        // 创建应用 #1
        QApplication app(argc, argv);
        QCoreApplication::setOrganizationName("DMC Studio");
        QCoreApplication::setOrganizationDomain(LINK_HOME_PAGE);
        QCoreApplication::setApplicationName(TEXT_PROGRAM_NAME_CN);

        // 安装翻译器 #2
        QTranslator translator;
        if (translator.load(":/i18n/lang/zh_cn.ts"))
        {
            app.installTranslator(&translator);
        }

        // 加载应用配置 #3
        registerSettings();
        if (!QFile::exists(FILE_CONFIG))
        {
            // + First-launch Notice
        }
        if (!FileOperator::loadConfig(FILE_CONFIG) || global_settings["version/file_version"].toInt() < FILEVERSION)
        {
            if (!FileOperator::saveConfig(FILE_CONFIG))
            {
                return 3;
            }
            // + Upgrade Notice
        }

        // 清理内存数据 #4
        initializeGlobalTable();

        // 加载 Table #5
        if (!FileOperator::loadTableData("default_table.json"))
        {
            if (!FileOperator::loadTableData("default_table.csv"))
                return 5;
        }
        global_p_ench_table = convertEnchTable(global_u_ench_table);

        // 创建应用GUI窗口 #6
        qDebug() << "Starting UI window...";
        MainWindow w;
        w.show();

        // 进入事件循环 #7
        if (app.exec() != RESTART_CODE)
            break;
    }

    FileOperator::saveConfig(FILE_CONFIG);
    return 0;
}

#endif

/*int main(int argc, char *argv[])
{

    if(argc > 1)    // 检查参数个数，若超过1个则不启动GUI
    {

//        // 定义命令解析实例，并设置其属性
//        QCommandLineParser parser;
//        parser.setApplicationDescription("BestEnchSeq made by Dinosaur_MC");
//        parser.addHelpOption();
//        parser.addVersionOption();

//        // 定义命令
//        QCommandLineOption console(QStringList() << "c" << "console", "以控制台模式启动");
//        parser.addOption(console);
//        QCommandLineOption test(QStringList() << "t" << "test", "Test Option");
//        parser.addOption(test);


//        parser.process(app);

//        // 命令实现
//        if(parser.isSet(console))   // 控制台模式
//        {
//            Launcher lunc(CORE, &app);
//            return lunc.launch();
//        }
//        else
//        {
//            if(!AttachConsole(ATTACH_PARENT_PROCESS)) // 捕获或创建控制台
//                AllocConsole();

//            // 重定向数据流
//            freopen("CONOUT$", "w", stdout);
//            freopen("CONOUT$", "w", stderr);
//            freopen("CONIN$", "r", stdin);
//        }

//        if(parser.isSet(test))  // 测试命令
//        {
//            cout << "Hello World" << endl;
//        }


//        // End
//        fclose(stdout);
//        fclose(stdin);
//        FreeConsole();
//        return 0;
//    }
//    else
//    {
//        QApplication app(argc, argv); // 创建带GUI的应用程序实例

//        int fontId = QFontDatabase::addApplicationFont(":/font/HarmonyOS_Sans_SC_Regular.ttf");    // 加载字体文件
//        if (fontId == -1)
//        {
//            qDebug() << "Load font failed!";
//        }
//        else
//        {
//            QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);    // 获取字体名称
//            if (fontName != nullptr)
//                app.setFont(fontName);
//        }

//        Launcher lunc(CORE, &app);
//        return lunc.launch();
    }
}*/
