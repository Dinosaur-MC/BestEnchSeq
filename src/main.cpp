#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QApplication>
#include <QTranslator>
#include "launcher.h"
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

        if (cmd.isSet("l"))
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
                while (launcher.launch(0) == RESTART_CODE)
                    ;
                return 0;
            }

            if (cmd.isSet("debug"))
            {
                qDebug() << "Developer mode";
                while (launcher.launch(2) == RESTART_CODE)
                    ;
                return 0;
            }
        }
    }

    qDebug() << "Graphics mode";
    while (launcher.launch(1) == RESTART_CODE)
        ;

    FileOperator::saveConfig(FILE_CONFIG);
    return 0;
}
