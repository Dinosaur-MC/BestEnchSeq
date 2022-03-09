#ifndef CHECKUPDATE_H
#define CHECKUPDATE_H

#include <QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class CheckUpdate : public QObject
{
    Q_OBJECT
public:
    explicit CheckUpdate(QObject *parent = nullptr);

    QNetworkAccessManager *manager;
    int UpdateJSON(QString str);
    void replyFinished(QNetworkReply *reply);

signals:

};

#endif // CHECKUPDATE_H
