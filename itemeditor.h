#ifndef ITEMEDITOR_H
#define ITEMEDITOR_H

#include <QDialog>
#include "basic.h"

QT_BEGIN_NAMESPACE
namespace Ui {class ItemEditor;}
QT_END_NAMESPACE

class ItemEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ItemEditor(QWidget *parent = nullptr);
    ~ItemEditor();

    Item item();

private:
    Ui::ItemEditor *ui;

signals:

};

#endif // ITEMEDITOR_H
