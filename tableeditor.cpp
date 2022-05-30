#include "tableeditor.h"
#include "ui_tableeditor.h"

TableEditor::TableEditor(QWidget *parent)
    : QDialog{parent},
      ui(new Ui::TableEditor)
{
    ui->setupUi(this);
    setWindowTitle("Table Editor");
    setFixedSize(320, 240);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=](){
        if(ui->tabWidget->currentIndex() == 0)
        {
            QString name = ui->leW_name->text();
            QString icon = ui->leW_icon->text();
        }
        else
        {

        }
        accept();
    });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &TableEditor::close);


    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](){
        if(ui->tabWidget->currentIndex() == 0)
        {
            setFixedSize(320, 240);
        }
        else
        {
            setFixedSize(480, 480);
        }
    });

    connect(ui->leW_icon, &QLineEdit::textChanged, this, [=](){
        QIcon icon(ui->leW_icon->text());
        if(icon.isNull())
            ui->toolButton->setIcon(QIcon());
        else
            ui->toolButton->setIcon(icon);
    });
}

TableEditor::~TableEditor()
{
    delete ui;
}
