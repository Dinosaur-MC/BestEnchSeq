#include "updatechecker.h"
#include "core/core.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QPushButton>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

UpdateChecker::UpdateChecker(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &UpdateChecker::readData);
    connect(manager, &QNetworkAccessManager::sslErrors, this, &UpdateChecker::failed);

    connect(this, &UpdateChecker::finished, this, [=](UpdateData data){
        qDebug() << "Read finished";
        if(data.available && data.version_id > VERSION_ID)
        {
            QString title = tr("【发现更新】 更新日期：") + data.update_time.toString("yyyy-MM-dd");
            QString content = tr("新版本：") + data.version_name + " (" + QString::number(data.version_id) + ")\n" + tr("更新说明：") + data.release_note + "\n" + tr("下载地址：") + data.url.toString();
            QMessageBox notice(QMessageBox::Icon::Information, title, content);
            QPushButton *getnow = notice.addButton(tr("立即获取"), QMessageBox::YesRole);
            notice.addButton(tr("下次一定"), QMessageBox::NoRole);
            notice.exec();
            if (notice.clickedButton() == getnow)
            {
                QDesktopServices::openUrl(data.url);
            }
        }
        else if(show_notice)
        {
            QMessageBox notice(QMessageBox::Icon::Information, tr("【无更新】 ") + VERSION_NAME, tr("当前版本已是最新版本！"), QMessageBox::Ok);
            notice.exec();
        }

        this->deleteLater();
    });

    connect(this, &UpdateChecker::failed, this, [=](){
        qDebug() << "Quest/Read failed";
        if(!show_notice)
        {
            this->deleteLater();
            return;
        }

        QString title = tr("【检测更新失败】JSON 解析失败");
        QString content = tr("更新服务器访问失败或JSON数据有误。\n请排查问题后重试或联系开发者修复错误。");
        QMessageBox notice(QMessageBox::Icon::Critical, title, content);
        QPushButton *retry = notice.addButton(tr("重试"), QMessageBox::ActionRole);
        QPushButton *contact = notice.addButton(tr("去拔开发者的头发"), QMessageBox::HelpRole);
        notice.addButton(tr("下次一定"), QMessageBox::RejectRole);
        notice.exec();
        if (notice.clickedButton() == retry)
        {
            check(url, true);
        }
        else if (notice.clickedButton() == contact)
        {
            QDesktopServices::openUrl(QUrl("https://github.com/Dinosaur-MC/BestEnchSeq/issues"));
            this->deleteLater();
        }
        else
        {
            this->deleteLater();
        }
    });
}

UpdateChecker::~UpdateChecker()
{
    delete manager;
}

void UpdateChecker::check(QUrl link, bool m)
{
    url = link;
    show_notice = m;
    quest();
}

void UpdateChecker::quest()
{
    qDebug() << "Questing for update...";
    QNetworkRequest request(url);
    manager->get(request);
}

void UpdateChecker::readData(QNetworkReply *reply)
{
    qDebug() << "Reading quest reply...";
    QByteArray data = reply->readAll().trimmed();
    qDebug() << data.data();
    reply->deleteLater();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if(err.error != QJsonParseError::NoError)
    {
        emit failed();
        return;
    }

    QJsonObject obj(doc.object());
    qDebug() << obj.keys().size() << obj.keys();
    if(obj.contains("1"))
    {
        QJsonObject ver = obj.value("1").toObject();
        if(ver.contains("LateseVersion"))
        {
            QJsonObject latest = ver.value("LatestVersion").toObject();
            if((latest.contains("Version") && latest.contains("VersionID")))
            {
                UpdateData data;
                data.version_name = latest.value("Version").toString();
                data.version_id = latest.value("VersionID").toInt();
                data.url = latest.value("Url").toString();
                data.update_time = QDateTime::fromString(latest.value("UpdateTime").toString(), "yyyy-MM-dd");
                data.release_note = latest.value("ReleaseNote").toString();
                data.available = latest.value("IsAvailable").toInt();
                emit finished(data);
                qDebug() << "JSON analysing done";
            }
            else
                emit failed();
        }
        else
            emit failed();
    }
    else
        emit failed();
}
