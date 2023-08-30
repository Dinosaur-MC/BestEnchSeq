#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QApplication>
#include <QTranslator>
#include "core/core.h"
#include "launcher.h"

int main(int argc, char *argv[])
{
    qDebug() << "(Application running)";

    // 创建应用实例并添加基本信息
    QApplication app(argc, argv);
    QApplication::setApplicationName(TEXT_PROGRAM_NAME);
    QApplication::setApplicationVersion(VERSION_NAME);
    QApplication::setOrganizationName("DMC Studio");
    QApplication::setOrganizationDomain(LINK_HOME_PAGE);

    // 加载默认语言文件并安装语言翻译器
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString name = QLocale(locale).name().toLower();
        if (translator.load(":/lang/" + name + ".qm"))
        {
            app.installTranslator(&translator);
            break;
        }
    }

    // 解析参数
    if (argc > 1)
    {
        QCommandLineParser cmd;
        cmd.addHelpOption();
        cmd.addVersionOption();
        cmd.setSingleDashWordOptionMode(QCommandLineParser::ParseAsCompactedShortOptions);
        cmd.addOptions({
            QCommandLineOption({"l", "license"}, QApplication::tr("Show the license of the application.")),
            QCommandLineOption({"c", "console"}, QApplication::tr("Launch as console mode.")),
            QCommandLineOption({"g", "graphics"}, QApplication::tr("Launch as graphics mode (default).")),
            QCommandLineOption("debug", QApplication::tr("Developer mode.")),
            QCommandLineOption({"r","read_only"}, QApplication::tr("Read only mode.")),
            QCommandLineOption({"s", "safe_mode"}, QApplication::tr("Run as safe mode. It will disable all file operation from the appliction."))
        });
        cmd.process(app);

        if (cmd.isSet("license"))
        {
            qDebug() << LICENSE;
            qDebug() << "(Application exiting)";
            return 0;
        }

        Launcher launcher;
        LaunchMode mode = LaunchMode::None;

        if (cmd.isSet("debug"))
            mode = (LaunchMode)((int)mode | (int)LaunchMode::DebugMode);
        if (cmd.isSet("read_only"))
            mode = (LaunchMode)((int)mode | (int)LaunchMode::ReadOnlyMode);
        if (cmd.isSet("safe_mode"))
            mode = (LaunchMode)((int)mode | (int)LaunchMode::SafeMode);

        if (cmd.isSet("console"))
            mode = (LaunchMode)((int)mode | (int)LaunchMode::ConsoleMode);
        if (cmd.isSet("graphics"))
            mode = (LaunchMode)((int)mode | (int)LaunchMode::GraphicsMode);

        while (launcher.launch(mode) == CODE_RESTART)
            ;
    }
    else
    {
        Launcher launcher;
        while (launcher.launch(LaunchMode::GraphicsMode) == CODE_RESTART)
            ;
    }

    qDebug() << "(Application exiting)";
    return 0;
}
