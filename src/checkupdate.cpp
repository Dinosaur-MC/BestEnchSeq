#include "checkupdate.h"
#include "core/core.h"

CheckUpdate::CheckUpdate(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &CheckUpdate::ReadData);
    connect(manager, &QNetworkAccessManager::sslErrors, this, &CheckUpdate::failed);
}

void CheckUpdate::setUrl(QUrl l)
{
    url = l;
}

void CheckUpdate::start(bool m)
{
    qDebug() << "<CheckUpdate>[info] Requesting...";
    show_notice = m;
    QNetworkRequest request(url);
    manager->get(request);
    qDebug() << "<CheckUpdate>[info] Requested.";
}

void CheckUpdate::ReadData(QNetworkReply *reply)
{
    QString str = reply->readAll();
    qDebug() << "<CheckUpdate>[info] " << str;
    AnalyseJSON(str);
    reply->deleteLater();
}

void CheckUpdate::AnalyseJSON(QString str)
{
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(str.toUtf8().data(), &err_rpt);

    if(err_rpt.error != QJsonParseError::NoError)
    {
        emit failed();
        if(show_notice)
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowTitle("错误 ERROR：JSON解析失败");
            msgBox.setText("服务器访问失败或JSON格式错误");
            msgBox.exec();
        }

        status = -1;
        emit finished();
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
            emit updateAvailable();
            status = 1;
            QString info = "发现更新！";
            info += "\n新版本：" + verison;
            info += "\n更新时间： " + update_time;
            info += "\n更新说明： " + release_note;
            msgBox.setText(info);
            msgBox.setInformativeText("是否现在获取更新？");
            QPushButton *yes = new QPushButton("立即获取");
            QPushButton *no = new QPushButton("下次一定");
            msgBox.addButton(no, QMessageBox::NoRole);
            msgBox.addButton(yes, QMessageBox::YesRole);
            msgBox.setDefaultButton(yes);
            msgBox.show();
            if(msgBox.exec() == QDialog::Accepted)
            {
                if(!QDesktopServices::openUrl(QUrl(url)))
                {
                    QMessageBox msgBox2;
                    msgBox2.setIcon(QMessageBox::Critical);
                    msgBox2.setWindowTitle("错误");
                    QString warn = "无法访问指定网址！\n";
                    msgBox2.setText(warn);
                    QPushButton *confirm = new QPushButton("确定");
                    msgBox2.addButton(confirm, QMessageBox::AcceptRole);
                    msgBox2.show();
                    msgBox2.exec();
                }
            }
        }
        else
        {
            emit noUpdate();
            if(show_notice)
            {
                QString info = "当前已是最新版本！";
                msgBox.setText(info);
                QPushButton *confirm = new QPushButton("确定");
                msgBox.addButton(confirm, QMessageBox::AcceptRole);
                msgBox.exec();
            }
            status = 0;
        }
    }

    emit finished();
    this->deleteLater();
}

