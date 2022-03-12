#include "checkupdate.h"

CheckUpdate::CheckUpdate(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &CheckUpdate::ReadData);
}


void CheckUpdate::start()
{
    qDebug() << "Requesting..";
    QNetworkRequest request(QUrl(UPDATE_JSON));
    manager->get(request);
    qDebug() << "Requested";
}

void CheckUpdate::ReadData(QNetworkReply *reply)
{
    qDebug() << "replyFinished";
    QString str = reply->readAll();
    qDebug() << "[ReadData]" << str;
    AnalyseJSON(str);
    reply->deleteLater();
    this->deleteLater();
}

void CheckUpdate::AnalyseJSON(QString str)
{
    qDebug() << "AnalyseJSON";
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(str.toUtf8().data(), &err_rpt);
    if(err_rpt.error != QJsonParseError::NoError)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("错误 Error：JSON解析失败");
        msgBox.setText("服务器访问失败或JSON格式错误");
        msgBox.exec();
        return;
    }
    if(root_Doc.isObject())
    {
        QJsonObject root_Obj = root_Doc.object();
        QJsonObject value = root_Obj.value("BestEnchSeq").toObject();

        QString verison = value.value("LatestVerison").toString();
        int verison_id = value.value("VerisonID").toInt();
        QString url = value.value("Url").toString();
        QString update_time = value.value("UpdateTime").toString();
        QString release_note = value.value("ReleaseNote").toString();

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("检查更新 Check Update");

        if(verison_id > VERSION_ID)
        {
            QString info = "发现更新！ Found update!";
            info += "新版本(Version)：" + verison;
            info += "更新时间(Date)：" + update_time;
            info += "更新说明(Note)：" + release_note;
            info += "\n";
            msgBox.setText(info);
            msgBox.setInformativeText("你想现在获取吗？\nDo you want to get it now?");
            QPushButton *no = new QPushButton("取消 Cancel");
            QPushButton *yes = new QPushButton("立即升级 Update");
            msgBox.addButton(no, QMessageBox::NoRole);
            msgBox.addButton(yes, QMessageBox::YesRole);
            if(msgBox.exec() == QMessageBox::YesRole)
            {
                QDesktopServices::openUrl(QUrl(url));
            }
        }
        else
        {
            QString info = "当前版本已是最新版本！\n";
            msgBox.setText(info);
            QPushButton *confirm = new QPushButton("确定 Confirm");
            msgBox.addButton(confirm, QMessageBox::AcceptRole);
            msgBox.exec();
        }
    }
}

