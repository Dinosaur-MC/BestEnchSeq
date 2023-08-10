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
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    while (1)
    {
        // 创建应用 #1
        QApplication app(argc, argv);
        QApplication::setOrganizationName("DMC Studio");
        QApplication::setOrganizationDomain(LINK_HOME_PAGE);
        QApplication::setApplicationName(TEXT_PROGRAM_NAME_CN);

        // 安装翻译器 #2
        QTranslator translator;
        if (translator.load("./lang/zh_cn.ts"))
        {
            app.installTranslator(&translator);
        }

        // 加载应用配置 #3
        if (!QFile::exists(FILE_CONFIG))
        {
            // + First-launch Notice
        }
        if (!FileOperator::loadConfig(FILE_CONFIG) || global_settings.file_version < FILEVERSION)
        {
            if (!FileOperator::saveConfig(FILE_CONFIG))
            {
                return 3;
            }
            // + Upgrade Notice
        }

        // 清理内存数据 #4
        current_table = DataTable();

        // 加载 Table #5
        if (!FileOperator::loadTableData("default_table.json", current_table))
        {
            if (!FileOperator::loadTableData("default_table.csv", current_table))
                return 5;
        }

        // 创建应用GUI窗口 #6
        qDebug() << "Starting GUI...";
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
