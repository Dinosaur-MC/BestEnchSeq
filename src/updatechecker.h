#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

struct UpdateData
{
    QString version_name;
    int version_id;
    QUrl url;
    QDateTime update_time;
    QString release_note;
    bool available;
};

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker(bool auto_delete);
    ~UpdateChecker();

    void check(QUrl link, bool m = true);

private:
    void quest();
    void readData(QNetworkReply *reply);

    QNetworkAccessManager manager;
    QUrl url;
    bool show_notice;
    bool auto_del;

signals:
    void dataRecieved();
    void finished(UpdateData data);
    void failed();
};

#endif // UPDATECHECKER_H
