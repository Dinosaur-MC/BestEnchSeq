#include "updatechecker.h"
#include "core/core.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QPushButton>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

UpdateChecker::UpdateChecker(bool auto_delete)
    : auto_del(auto_delete)
{
    connect(&manager, &QNetworkAccessManager::finished, this, &UpdateChecker::readData);
    connect(&manager, &QNetworkAccessManager::sslErrors, this, &UpdateChecker::failed);

    connect(this, &UpdateChecker::finished, this, [=](UpdateData data)
        {
            qDebug() << "[UpdateChecker] Finished.";
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
                    if (!QDesktopServices::openUrl(data.url))
                        QDesktopServices::openUrl(QUrl("https://github.com/Dinosaur-MC/BestEnchSeq/releases"));
                }
            }
            else if(show_notice)
            {
                QMessageBox notice(QMessageBox::Icon::Information, tr("【无更新】 ") + VERSION_NAME, tr("当前版本已是最新版本！"), QMessageBox::Ok);
                notice.exec();
            }

            if (auto_del)
                this->deleteLater();
        });

    connect(this, &UpdateChecker::failed, this, [=]()
        {
            qDebug() << "[UpdateChecker] Quest/Read failed.";
            if(show_notice)
            {
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
                }
            }

            if (auto_del)
                this->deleteLater();
        });
}

UpdateChecker::~UpdateChecker()
{
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
    manager.get(request);
}

void UpdateChecker::readData(QNetworkReply *reply)
{
    qDebug() << "Reading quest reply...";
    QByteArray data = reply->readAll().trimmed();
    qDebug() << data.data();
    reply->deleteLater();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError)
    {
        emit failed();
        return;
    }

    QJsonObject obj(doc.object());
    if (obj.contains("1"))
    {
        QJsonObject ver = obj.value("1").toObject();
        if (ver.contains("LatestVersion"))
        {
            QJsonObject latest = ver.value("LatestVersion").toObject();
            if ((latest.contains("Version") && latest.contains("VersionID")))
            {
                UpdateData data;
                data.version_name = latest.value("Version").toString();
                data.version_id = latest.value("VersionID").toInt();
                data.url = latest.value("Url").toString();
                data.update_time = QDateTime::fromString(latest.value("UpdateTime").toString(), "yyyy-MM-dd");
                data.release_note = latest.value("ReleaseNote").toString();
                data.available = latest.value("IsAvailable").toBool();
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
