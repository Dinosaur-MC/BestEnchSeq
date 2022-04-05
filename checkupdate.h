#ifndef CHECKUPDATE_H
#define CHECKUPDATE_H

#include <QObject>
#include <QMessageBox>
#include <QPushButton>

#include <QDesktopServices>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "base.h"


class CheckUpdate : public QObject
{
    Q_OBJECT
public:
    explicit CheckUpdate(QObject *parent = nullptr);

    void start(bool m); //获取更新
    void ReadData(QNetworkReply *reply);    //接收并读取返回数据
    void AnalyseJSON(QString str);  //JSON解析 & 弹窗提示

private:
    QNetworkAccessManager *manager;
    bool notice_newest;

signals:
    void finished();

};

#endif // CHECKUPDATE_H
