#include "itemeditor.h"
#include "ui_itemeditor.h"

ItemEditor::ItemEditor(QWidget *parent)
    : QDialog{parent}
    , ui(new Ui::ItemEditor)
{
    ui->setupUi(this);

}

ItemEditor::~ItemEditor()
{

}


Item ItemEditor::item()
{
    return Item();
}

