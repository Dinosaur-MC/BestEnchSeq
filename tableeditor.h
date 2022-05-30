#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {class TableEditor;}
QT_END_NAMESPACE

class TableEditor : public QDialog
{
    Q_OBJECT
public:
    explicit TableEditor(QWidget *parent = nullptr);
    ~TableEditor();

private:
    Ui::TableEditor *ui;

};

#endif // TABLEEDITOR_H
