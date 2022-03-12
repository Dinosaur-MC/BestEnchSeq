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

    void start();
    void ReadData(QNetworkReply *reply);
    void AnalyseJSON(QString str);

private:
    QNetworkAccessManager *manager;

signals:

};

#endif // CHECKUPDATE_H
