#ifndef CHECKUPDATE_H
#define CHECKUPDATE_H

#include <QObject>
#include <QMessageBox>
#include <QPushButton>
#include "basic.h"

#include <QDesktopServices>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class CheckUpdate : public QObject
{
    Q_OBJECT
public:
    explicit CheckUpdate(QObject *parent = nullptr);

    void start(bool m);
    void ReadData(QNetworkReply *reply);
    void AnalyseJSON(QString str);

private:
    QNetworkAccessManager *manager;
    bool notice_newest;

signals:
    void finished();

};

#endif // CHECKUPDATE_H
