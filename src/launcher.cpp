#include "launcher.h"
#include <windows.h>
#include <QApplication>
#include <QMessageBox>
#include <QDesktopServices>
#include "core/fileoperator.h"
#include "langs/languagemgr.h"
#include "updatechecker.h"
#include "ui/console.h"
#include "ui/graphics.h"

Launcher::~Launcher()
{
}

int Launcher::launch(LaunchMode mode)
{
    // 加载配置
    global_settings = defaultSettings();
    if (!QFile::exists(FILE_CONFIG))
    {
        qDebug() << "[Warning] Cannot found file 'config.ini', creating by the default.";
        QMessageBox notice(QMessageBox::Icon::Warning, tr("【使用说明】必读"), tr(STATEMENT), QMessageBox::Yes | QMessageBox::Cancel);
        if (notice.exec() == QMessageBox::Cancel)
            return 0;

        if (!FileOperator::saveConfig(FILE_CONFIG))
        {
            qDebug() << "[Error] Cannot write the configuration file.";
            return 1;
        }
    }
    else
    {
        if (!FileOperator::loadConfig(FILE_CONFIG))
        {
            qDebug() << "[Error] Cannot load the configuration file.";
            return 1;
        }
    }

    // 切换语言
    if (global_settings.language.isEmpty())
    {
        if (global_lang_mgr.adaptLangauge())
            global_settings.language = global_lang_mgr.currentLanguage();
    }
    else
    {
        if (!global_lang_mgr.setLanguage(global_settings.language))
        {
            if (global_lang_mgr.adaptLangauge())
                global_settings.language = global_lang_mgr.currentLanguage();
        }
    }

    // 版本检测
    if (global_settings.app_version < VERSION_ID || global_settings.file_version < FILEVERSION)
    {
        FileOperator::saveConfig(FILE_CONFIG);
        QMessageBox notice(QMessageBox::Icon::Information, tr("【更新完成】") + VERSION_NAME + " - " + QString::number(VERSION_ID), tr("【更新日志】\n"), QMessageBox::Ok);
        notice.exec();
        global_settings.app_version = VERSION_ID;
        global_settings.file_version = FILEVERSION;
    }

    // 检查更新
    if (global_settings.auto_check_update)
    {
        qDebug() << "[Info] Checking update";
        UpdateChecker *update = new UpdateChecker(true);
        update->check(QUrl(LINK_UPDATE_DATA), false);
    }

    // 按指定模式创建并启动主实例
    qDebug() << "[Info] Start main program";
    int main_mode = (int)mode & 0x0F;
    int addtion = (int)mode & 0xF0;

    if (addtion & (int)LaunchMode::DebugMode)
    {
        if (AttachConsole(ATTACH_PARENT_PROCESS)) // 捕获控制台
        {
            // 重定向输出流至控制台
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
            printf("--- Debug Console Started ---\n");
        }
    }
    if (addtion & (int)LaunchMode::ReadOnlyMode)
    {
        FileOperator::mode = 0x1;
    }
    if (addtion & (int)LaunchMode::SafeMode)
    {
        FileOperator::mode = 0x0;
    }

    int code;
    if ((main_mode & (int)LaunchMode::ConsoleMode) > 0 && (main_mode & !(int)LaunchMode::ConsoleMode) == 0)
    {
        Console console;
        code = console.run();
    }
    else if ((main_mode & (int)LaunchMode::GraphicsMode) > 0 && (main_mode & !(int)LaunchMode::GraphicsMode) == 0)
    {
        Graphics graphics;
        code = graphics.run();
    }
    else
        return 2;

    FileOperator::saveConfig(FILE_CONFIG);
    return code;
}
