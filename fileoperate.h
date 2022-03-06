#ifndef FILEOPERATE_H
#define FILEOPERATE_H

#include <QObject>

class FileOperate : public QObject
{
    Q_OBJECT
public:
    explicit FileOperate(QObject *parent = nullptr);

    void saveConfig();
    void saveWeapon();
    void saveEnchantmentTable();

    void loadConfig();
    void loadWeapon();
    void loadEnchantmentTable();

    void saveExport();

signals:

};

#endif // FILEOPERATE_H
