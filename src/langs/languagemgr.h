#ifndef LANGUAGEMGR_H
#define LANGUAGEMGR_H

#include <QTranslator>

class LanguageMgr : public QObject
{
    Q_OBJECT
public:
    explicit LanguageMgr(QObject *parent = nullptr);

    bool adaptLangauge();
    bool setLanguage(QString short_name);
    QString currentLanguage() const;

private:
    QTranslator translator;
    QString current_language;

signals:
    void languageChanged(QString name);
};

extern LanguageMgr global_lang_mgr;

#endif // LANGUAGEMGR_H
