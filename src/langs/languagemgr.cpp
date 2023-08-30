#include "languagemgr.h"
#include <QApplication>
#include <QLocale>

LanguageMgr global_lang_mgr;

LanguageMgr::LanguageMgr(QObject *parent)
    : QObject{parent}
{
}

QStringList LanguageMgr::langaugeNameList()
{
    QStringList list;
    list.append("简体中文");
    list.append("繁體中文");
    list.append("文言");
    list.append("English");
    return list;
}

QStringList LanguageMgr::langaugeShortNameList()
{
    QStringList list;
    list.append("zh_cn");
    list.append("zh_tw");
    list.append("lzh_cn");
    list.append("en_us");
    return list;
}

bool LanguageMgr::adaptLangauge()
{
    bool success = false;

    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString name = QLocale(locale).name().toLower();
        if (translator.load(":/langs/" + name + ".qm"))
        {
            QApplication::installTranslator(&translator);
            current_language = name;
            success = true;
            break;
        }
    }
    if (!success && translator.load(":/langs/en_us.qm"))
    {
        QApplication::installTranslator(&translator);
        current_language = "en_us";
        success = true;
    }

    if (success)
    {
        qDebug() << "[LanguageMgr] Language switched to " + current_language + ".";
        emit languageChanged(current_language);
    }
    return success;
}

bool LanguageMgr::setLanguage(QString short_name)
{
    if (translator.load(":/langs/" + short_name + ".qm"))
    {
        QApplication::installTranslator(&translator);
        current_language = short_name;
        qDebug() << "[LanguageMgr] Language switched.";
        emit languageChanged(current_language);
        return true;
    }
    else
    {
        qDebug() << "[LanguageMgr] Failed to switch the language to " + current_language + ".";
        return false;
    }
}

QString LanguageMgr::currentLanguage() const
{
    return current_language;
}
