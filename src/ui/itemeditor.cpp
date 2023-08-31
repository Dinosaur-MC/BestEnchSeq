#include "itemeditor.h"
#include "ui_itemeditor.h"

ItemEditor::ItemEditor(const Group &g, QWidget *parent)
    : QDialog{parent},
    group(g),
    ui(new Ui::ItemEditor)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Edit Item"));
    this->setFont(QFont("微软雅黑", 12));

    refresh();

    connect(ui->radio_book, &QRadioButton::clicked, this, [=](bool checked){
        if (checked)
        {
            type = ItemType::Book;
            ui->listWidget->setEnabled(true);
            ui->label_durability->setEnabled(false);
            ui->sb_durability->setEnabled(false);
        }
    });
    connect(ui->radio_weapon, &QRadioButton::clicked, this, [=](bool checked){
        if (checked)
        {
            type = ItemType::Weapon;
            ui->listWidget->setEnabled(true);
            ui->label_durability->setEnabled(true);
            ui->sb_durability->setEnabled(true);
        }
    });
    connect(ui->radio_stuff, &QRadioButton::clicked, this, [=](bool checked){
        if (checked)
        {
            type = ItemType::Stuff;
            ui->listWidget->setEnabled(false);
            ui->label_durability->setEnabled(false);
            ui->sb_durability->setEnabled(false);
        }
    });

    connect(ui->btn_ok, &QPushButton::clicked, this, [=](){
        if (ui->radio_book->isChecked() && ui->listWidget->getEnchList().isEmpty())
            return;
        this->accept();
    });
    connect(ui->btn_cancel, &QPushButton::clicked, this, &ItemEditor::reject);
}

ItemEditor::~ItemEditor()
{
    delete ui;
}

void ItemEditor::setGroup(const Group &g)
{
    group = g;
    refresh();
}

void ItemEditor::setItem(Item item)
{
    _item = item;

    if (_item.type == ItemType::Book)
        ui->radio_book->setChecked(true);
    else if (_item.type == ItemType::Weapon)
        ui->radio_weapon->setChecked(true);
    else
        ui->radio_stuff->setChecked(true);

    for (const Ench &e: _item.enchs)
    {
        for (int i = 0l; i < group.enchantments.size(); i++)
        {
            if (e.name == group.enchantments.at(i).name)
            {
                ui->listWidget->setChecked(i, true);
                ui->listWidget->setEnchLevel(i, e.lvl);
                break;
            }
        }
    }

    ui->sb_penalty->setValue(_item.penalty);
    ui->sb_durability->setMaximum(group.max_durability);
    ui->sb_durability->setValue(_item.durability);
}

void ItemEditor::refresh()
{
    type = ItemType::Book;
    ui->radio_book->setChecked(true);
    ui->listWidget->setEnabled(true);
    ui->label_durability->setEnabled(false);
    ui->sb_durability->setEnabled(false);
    ui->listWidget->setEnchData(group.enchantments);
    ui->sb_penalty->setValue(0);
    ui->sb_durability->setValue(group.max_durability);
    ui->sb_count->setValue(1);
}

ItemPool ItemEditor::getItem()
{
    _item.type = type;
    _item.penalty = ui->sb_penalty->value();

    if (_item.type == ItemType::Book)
    {
        _item.durability = 0;
        _item.enchs = ui->listWidget->getEnchList();
    }
    else if (_item.type == ItemType::Weapon)
    {
        _item.durability = ui->sb_durability->value();
        _item.enchs = ui->listWidget->getEnchList();
    }
    else
    {
        _item.durability = ui->sb_count->value();
        ItemPool ip;
        ip.append(_item);
        return ip;
    }

    ItemPool ip;
    for (int i = 0; i < ui->sb_count->value(); i++)
        ip.append(_item);
    return ip;
}
