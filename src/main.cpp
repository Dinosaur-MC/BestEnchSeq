#define BUILD_GUI 1
//#include "core/core.h"
//#include "core/fileoperator.h"
#include "launcher.h"

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
        current_table = DataTable();

        // 加载 Table #4
        if (!FileOperator::loadTableData("default_table.json", current_table))
        {
            if (!FileOperator::loadTableData("default_table.csv", current_table))
                return 5;
        }

        // 启动 Console 程序

        app.exec();
    }
    return 0;
}

#endif

#if BUILD_GUI == 1
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QApplication>
#include <QTranslator>
#include "core/fileoperator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(QApplication::tr("The Best Enchanting Sequence"));
    QApplication::setApplicationVersion(VERSION_NAME);
    QApplication::setOrganizationName("DMC Studio");
    QApplication::setOrganizationDomain(LINK_HOME_PAGE);

    QTranslator translator;
    if (translator.load(":/lang/zh_cn.qm"))
    {
        app.installTranslator(&translator);
        qDebug() << "Translator installed.";
    }

    Launcher launcher;

    if (argc > 1)
    {
        QCommandLineParser cmd;
        cmd.addHelpOption();
        cmd.addVersionOption();
        cmd.setSingleDashWordOptionMode(QCommandLineParser::ParseAsCompactedShortOptions);
        cmd.addOptions({QCommandLineOption("m", QApplication::tr("Run as command mode (not available yet).")),
                        QCommandLineOption({"c", "console"}, QApplication::tr("Launch as console mode.")),
                        QCommandLineOption({"g", "graphics"}, QApplication::tr("Launch as graphics mode (default).")),
                        QCommandLineOption({"l", "license"}, QApplication::tr("Show the license of the program.")),
                        QCommandLineOption("debug", QApplication::tr("Developer mode."))});
        cmd.process(app);

        if(cmd.isSet("l"))
        {
            qDebug() << LICENSE;
            return 0;
        }

        if (cmd.isSet("m"))
        {
            qDebug() << "Command mode";
        }
        else
        {
            if (cmd.isSet("console") && cmd.isSet("graphics"))
            {
                qDebug() << "[Error] Option 'console' conflicts with 'graphics'.";
                return 1;
            }

            if (cmd.isSet("console"))
            {
                qDebug() << "Console mode";
                while(launcher.launch(0) == RESTART_CODE);
                return 0;
            }

            if (cmd.isSet("debug"))
            {
                qDebug() << "Developer mode";
                while(launcher.launch(2) == RESTART_CODE);
                return 0;
            }
        }
    }

    qDebug() << "Graphics mode";
    while(launcher.launch(1) == RESTART_CODE);

    FileOperator::saveConfig(FILE_CONFIG);
    return 0;
}

#endif
