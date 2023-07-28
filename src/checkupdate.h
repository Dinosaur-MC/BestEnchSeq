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

class CheckUpdate : public QObject
{
    Q_OBJECT
public:
    explicit CheckUpdate(QObject *parent = nullptr);

    void setUrl(QUrl l);

    void start(int verison_id, bool m);  // 获取更新
    void ReadData(QNetworkReply *reply); // 接收并读取返回数据
    void AnalyseJSON(QString str);       // JSON解析 & 弹窗提示

    int status;

private:
    QNetworkAccessManager *manager;
    bool show_notice;
    QUrl url;
    int target_verison_id;

signals:
    void failed();
    void updateAvailable();
    void noUpdate();
    void finished();
};

#endif // CHECKUPDATE_H
