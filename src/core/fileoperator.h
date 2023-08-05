#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include "core.h"

class FileOperator : public QObject
{
    Q_OBJECT
public:
    explicit FileOperator() = default;
    ~FileOperator() = default;

    static bool loadConfig(QString file_name);
    static bool saveConfig(QString file_name);

    static bool loadTableData(QString file_name, bool m = true);
    static bool saveTableData(QString file_name);

    static bool loadArchivePoint(QString file_name);
    static bool saveArchivePoint(QString file_name);

    static bool saveResult(Summary summary, QList<FlowStep> flow, QString file_name, int mode = 0);
};

#endif // FILEOPERATOR_H
