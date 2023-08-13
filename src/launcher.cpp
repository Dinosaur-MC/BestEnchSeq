#include "launcher.h"
#include <QApplication>
#include <QMessageBox>
#include <QDesktopServices>
#include "core/fileoperator.h"
#include "windows.h"

Launcher::~Launcher()
{

}

int Launcher::launch(int mode)
{
    // 加载配置
    global_settings = defaultSettings();
    if (!QFile::exists(FILE_CONFIG))
    {
        qDebug() << "[Warning] Cannot found file 'config.ini', creating by the default.";
        QMessageBox notice(QMessageBox::Icon::Warning, tr("【使用说明】必读"), tr(STATEMENT), QMessageBox::Yes | QMessageBox::Cancel);
        if(notice.exec() == QMessageBox::Cancel)
            return 0;

        if(!FileOperator::saveConfig(FILE_CONFIG))
        {
            qDebug() << "[Error] Cannot write the configuration file.";
            return 1;
        }
    }
    else
    {
        if(!FileOperator::loadConfig(FILE_CONFIG))
        {
            qDebug() << "[Error] Cannot load the configuration file.";
            return 1;
        }
    }

    // 版本检测
    if(global_settings.app_version < VERSION_ID || global_settings.file_version < FILEVERSION)
    {
        FileOperator::saveConfig(FILE_CONFIG);
        QMessageBox notice(QMessageBox::Icon::Information, tr("【更新完成】") + VERSION_NAME + " - " + QString::number(VERSION_ID), tr(""), QMessageBox::Ok);
        notice.exec();
        global_settings.app_version = VERSION_ID;
        global_settings.file_version = FILEVERSION;
    }

    // 检查更新
    if (global_settings.auto_check_update)
    {
        qDebug() << "[Launcher] Checking update";
        update = new UpdateChecker(this);
        update->check(QUrl(LINK_UPDATE_DATA), true);
    }

    // 启动计算器实例
    qDebug() << "[Launcher] Start program";
    if (mode == 0)
    {
        console = new Console(this);
        return console->run();
    }
    else if (mode == 1)
    {
        graphics = new Graphics(this);
        return graphics->run();
    }
    else if(mode == 2)
    {
        if(AllocConsole()) // 启动控制台
        {
            // 重定向输出流至控制台
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
            printf("--- Debug Console Started ---\n");

            // 启动 GUI 实例
            graphics = new Graphics();
            int ret = graphics->run();

            // 结束并释放控制台
            printf("--- Debug Console Exited ---\n");
            fclose(stdout);
            fclose(stderr);
            FreeConsole();

            return ret;
        }
    }
    else
        return 2;

    return 0;
}

void Launcher::stop()
{
    if(console)
    {
        console->exit(0);
        delete console;
        console = nullptr;
    }
    if(graphics)
    {
        graphics->exit(0);
        delete graphics;
        graphics = nullptr;
    }

    table_mgr.empty();
    global_settings = defaultSettings();
}

int Launcher::restart()
{
    return 0;
}
