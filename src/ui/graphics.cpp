#include "graphics.h"
#include "ui_graphics.h"
#include "core/tablemanager.h"

#include <QApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTimer>

#include <QBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>

#include <QGroupBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QListWidget>
#include <QListWidgetItem>

#include <QRadioButton>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QComboBox>
#include <QProgressBar>

Graphics::Graphics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Graphics)
{
    // Initialize Variable
    global_table_mgr.loadDefaultTable();    // 加载默认Table
    table_info_list = global_table_mgr.getAllTabeInfo();    // 获取Table信息
    current_table = global_table_mgr.getTable();    // 更新当前Table

    if (!current_table.groups.isEmpty())    // 更新Group选项
        selected_group_name = current_table.groups.at(0).name;

    algorithm_list = alg_manager.getInternalAlgorithm();    // 载入内置算法
    algorithm_list.append(alg_manager.detectAlgorithm());   // 探测外部算法

    current_widget = -1;
    memset(stored_pos, 0, 4 * sizeof(int));

    // Setup UI
    ui->setupUi(this);
    this->setWindowTitle(tr(TEXT_PROGRAM_NAME));

    setupPatterns();

    // Set Default
    switchTab(0);
}

Graphics::~Graphics()
{
    delete ui;
}

int Graphics::run()
{
    qDebug() << "Graphics running";

    this->show();
    int ret = QApplication::exec();

    qDebug() << "Graphics exiting";
    return ret;
}

void Graphics::setupPatterns()
{
    // Topbar
    ui->btn_app_icon->setIcon(QIcon(":/icon/logo.png"));
    ui->btn_app_icon->setIconSize(QSize(48, 48));
    ui->label_app_name->setText(QString(TEXT_PROGRAM_NAME_ABBR) + " " + VERSION_NAME);
    ui->label_app_name->setFont(QFont("得意黑", 24, -1, true));

    connect(ui->btn_app_icon, &QToolButton::clicked, this, [=](){
        QMessageBox notice(QMessageBox::Icon::Information, tr("Opening URL"), tr("Are you sure to open this URL?\n") + LINK_HOME_PAGE);
        notice.addButton(QMessageBox::Open);
        notice.addButton(QMessageBox::Cancel);
        if(notice.exec() == QMessageBox::Open)
            QDesktopServices::openUrl(QUrl(LINK_HOME_PAGE));
    });
    connect(ui->btn_calc, &QToolButton::clicked, this, [=](){
        switchTab(0);
    });
    connect(ui->btn_table, &QToolButton::clicked, this, [=](){
        switchTab(1);
    });
    connect(ui->btn_tool, &QToolButton::clicked, this, [=](){
        switchTab(2);
    });
    connect(ui->btn_conf, &QToolButton::clicked, this, [=](){
        switchTab(3);
    });

    /* ------------------------------------------
     *             Sidebar & Content
     * ------------------------------------------ */

    // ------------ Content Layout ------------
    ui->label_text->setAlignment(Qt::AlignTop);

    QVBoxLayout *content_layout = new QVBoxLayout;
    content_layout->setAlignment(Qt::AlignTop);
    ui->scrollArea->widget()->setLayout(content_layout);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QTimer *timer = new QTimer(this);
    connect(this, &Graphics::currentTabChanged, this, [=](){
        timer->start(1);
    });
    connect(timer, &QTimer::timeout, this, [=](){
        timer->stop();
        ui->scrollArea->verticalScrollBar()->setValue(stored_pos[current_widget]);
        qDebug() << "Stored Pos [R]:" << current_widget << stored_pos[current_widget];
    });

    // ------------ Calculate Tab ------------
    QList<QWidget *> group_calc;

    // ********** Part 1 **********

    // Build Widget
    QGridLayout *l_calc_1 = new QGridLayout;
    QWidget *w_calc_1 = new QWidget;
    w_calc_1->setLayout(l_calc_1);
    w_calc_1->hide();

    // Edition Selector
    QHBoxLayout *gbL_1_1 = new QHBoxLayout;
    QGroupBox *gb_1_1 = new QGroupBox(tr("Select Edition"));
    gb_1_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_1_1->setLayout(gbL_1_1);

    QRadioButton *radio_mce_java = new QRadioButton(tr("Java"));
    radio_mce_java->setChecked(true);
    QRadioButton *radio_mce_bedrock = new QRadioButton(tr("Bedrock"));

    gbL_1_1->addWidget(radio_mce_java);
    gbL_1_1->addWidget(radio_mce_bedrock);
    l_calc_1->addWidget(gb_1_1, 0, 0, 1, 4);

    // Table Selector
    QHBoxLayout *gbL_1_2 = new QHBoxLayout;
    QGroupBox *gb_1_2 = new QGroupBox(tr("Select Table"));
    gb_1_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_1_2->setLayout(gbL_1_2);

    QComboBox *cb_1_1 = new QComboBox(gb_1_2);
    cb_1_1->setIconSize(QSize(48, 48));
    foreach (auto &info, table_info_list)
        cb_1_1->addItem(info.file_name.endsWith(".json") ? QIcon(":/icon/json.svg"): QIcon(":/icon/csv.svg"), info.file_name);

    gbL_1_2->addWidget(cb_1_1);
    l_calc_1->addWidget(gb_1_2, 1, 0, 1, 2);

    // Group Selector
    QHBoxLayout *gbL_1_3 = new QHBoxLayout;
    QGroupBox *gb_1_3 = new QGroupBox(tr("Select Group"));
    gb_1_3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_1_3->setLayout(gbL_1_3);

    QComboBox *cb_1_2 = new QComboBox(gb_1_3);
    cb_1_2->setIconSize(QSize(48, 48));
    foreach (auto &group, current_table.groups)
        cb_1_2->addItem(QIcon(group.icon_path), group.name);

    gbL_1_3->addWidget(cb_1_2);
    l_calc_1->addWidget(gb_1_3, 1, 2, 1, 2);

    // ICM Selector
    QHBoxLayout *gbL_1_4 = new QHBoxLayout;
    QGroupBox *gb_1_4 = new QGroupBox(tr("Select Item Configuring Method"));
    gb_1_4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_1_4->setLayout(gbL_1_4);

    QRadioButton *radio_icm_normal = new QRadioButton(tr("Normal"));
    radio_icm_normal->setChecked(true);
    QRadioButton *radio_icm_poor = new QRadioButton(tr("Poor"));
    QRadioButton *radio_icm_advanced = new QRadioButton(tr("Advanced"));

    gbL_1_4->addWidget(radio_icm_normal);
    gbL_1_4->addWidget(radio_icm_poor);
    gbL_1_4->addWidget(radio_icm_advanced);
    l_calc_1->addWidget(gb_1_4, 2, 0, 1, 2);

    // Apply Widget
    content_layout->addWidget(w_calc_1);
    group_calc.append(w_calc_1);


    // ********** Part 2 **********

    // Build Widget
    QGridLayout *l_calc_2 = new QGridLayout;
    QWidget *w_calc_2 = new QWidget;
    w_calc_2->setLayout(l_calc_2);
    w_calc_2->hide();

    // Origin Enchantment List
    QHBoxLayout *gbL_2_1 = new QHBoxLayout;
    QGroupBox *gb_2_1 = new QGroupBox(tr("Adjust Origin Enchantments"));
    gb_2_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_2_1->setLayout(gbL_2_1);

    QListWidget *origin_ench_list_widget = new QListWidget;
    origin_ench_list_widget->setMinimumHeight(500);
    foreach (auto &group, current_table.groups)
    {
        if (group.name == selected_group_name)
        {
            foreach (auto &ench, group.enchantments)
            {
                QHBoxLayout *layout = new QHBoxLayout;
                QWidget *item_widget = new QWidget;
                item_widget->setLayout(layout);

                QCheckBox *chkb = new QCheckBox;
                QLabel *label = new QLabel(ench.name);
                label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
                QComboBox *cbb = new QComboBox();
                for (int i = 1; i <= ench.max_lvl; i++)
                    cbb->addItem(QString::number(i));
                if(cbb->count() > 0)
                    cbb->setCurrentIndex(0);
                else
                    item_widget->setDisabled(true);

                layout->addWidget(chkb);
                layout->addWidget(label);
                layout->addWidget(cbb);

                QListWidgetItem *item = new QListWidgetItem;
                item->setSizeHint(item_widget->sizeHint());
                origin_ench_list_widget->addItem(item);
                origin_ench_list_widget->setItemWidget(item, item_widget);
            }
        }
    }

    gbL_2_1->addWidget(origin_ench_list_widget);
    l_calc_2->addWidget(gb_2_1);

    // Required Enchantment List
    QHBoxLayout *gbL_2_2 = new QHBoxLayout;
    QGroupBox *gb_2_2 = new QGroupBox(tr("Choose Required Enchantments"));
    gb_2_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_2_2->setLayout(gbL_2_2);

    QListWidget *required_ench_list_widget = new QListWidget;
    required_ench_list_widget->setMinimumHeight(500);
    foreach (auto &group, current_table.groups)
    {
        if (group.name == selected_group_name)
        {
            foreach (auto &ench, group.enchantments)
            {
                QHBoxLayout *layout = new QHBoxLayout;
                QWidget *item_widget = new QWidget;
                item_widget->setLayout(layout);

                QCheckBox *chkb = new QCheckBox;
                QLabel *label = new QLabel(ench.name);
                label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
                QComboBox *cbb = new QComboBox();
                for (int i = 1; i <= ench.max_lvl; i++)
                    cbb->addItem(QString::number(i));
                if(cbb->count() > 0)
                    cbb->setCurrentIndex(cbb->count()-1);
                else
                    item_widget->setDisabled(true);

                layout->addWidget(chkb);
                layout->addWidget(label);
                layout->addWidget(cbb);

                QListWidgetItem *item = new QListWidgetItem;
                item->setSizeHint(item_widget->sizeHint());
                required_ench_list_widget->addItem(item);
                required_ench_list_widget->setItemWidget(item, item_widget);
            }
        }
    }

    gbL_2_2->addWidget(required_ench_list_widget);
    l_calc_2->addWidget(gb_2_2);

    // Apply Widget
    content_layout->addWidget(w_calc_2);
    group_calc.append(w_calc_2);

    // ********** Part 3 **********

    // Build Widget
    QGridLayout *l_calc_3 = new QGridLayout;
    QWidget *w_calc_3 = new QWidget;
    w_calc_3->setLayout(l_calc_3);
    w_calc_3->hide();

    QHBoxLayout *gbL_3_1 = new QHBoxLayout;
    QGroupBox *gb_3_1 = new QGroupBox(tr("Edit Item Pool"));
    gb_3_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_3_1->setLayout(gbL_3_1);

    QListWidget *item_pool_list_widget = new QListWidget;
    item_pool_list_widget->setMinimumHeight(500);
    item_pool_list_widget->addItem(tr("(Click To Add Item)"));

    gbL_3_1->addWidget(item_pool_list_widget);
    l_calc_3->addWidget(gb_3_1);

    // Apply Widget
    content_layout->addWidget(w_calc_3);
    group_calc.append(w_calc_3);

    // ********** Part 4 **********

    // Build Widget
    QGridLayout *l_calc_4 = new QGridLayout;
    QWidget *w_calc_4 = new QWidget;
    w_calc_4->setLayout(l_calc_4);
    w_calc_4->hide();

    // Algorithm Selector
    QHBoxLayout *gbL_4_1 = new QHBoxLayout;
    QGroupBox *gb_4_1 = new QGroupBox(tr("Select Algorithm"));
    gb_4_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_4_1->setLayout(gbL_4_1);

    QToolButton *btn_4_1 = new QToolButton;
    btn_4_1->setIcon(QIcon(":/icon/shuaxin.svg"));
    btn_4_1->setIconSize(QSize(32, 32));
    QComboBox *cbb_4_1 = new QComboBox;
    foreach (auto &item, algorithm_list)
        cbb_4_1->addItem(item.name);
    selected_algorithm = algorithm_list.at(0).name;
    QLabel *label_4_1 = new QLabel;
    label_4_1->setOpenExternalLinks(true);
    foreach (auto &item, algorithm_list)
    {
        if(selected_algorithm == item.name)
            label_4_1->setText("V" + item.version + " <a href=\"" + item.link + "\">@" + item.author);
    }

    gbL_4_1->addWidget(btn_4_1);
    gbL_4_1->addWidget(cbb_4_1);
    gbL_4_1->addWidget(label_4_1);
    l_calc_4->addWidget(gb_4_1);

    // ALG Configuration Editor
    QHBoxLayout *gbL_4_2 = new QHBoxLayout;
    QGroupBox *gb_4_2 = new QGroupBox(tr("Add ALG Option"));
    gb_4_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_4_2->setLayout(gbL_4_2);

    QCheckBox *chkb_4_1 = new QCheckBox(tr("NoPlenalty"));
    QCheckBox *chkb_4_2 = new QCheckBox(tr("NoRepairing"));
    QCheckBox *chkb_4_3 = new QCheckBox(tr("IgnoreConflicting"));
    QCheckBox *chkb_4_4 = new QCheckBox(tr("UnlimitedForgeCount"));

    gbL_4_2->addWidget(chkb_4_1);
    gbL_4_2->addWidget(chkb_4_2);
    gbL_4_2->addWidget(chkb_4_3);
    gbL_4_2->addWidget(chkb_4_4);
    l_calc_4->addWidget(gb_4_2);

    // Start Calculate Button
    QVBoxLayout *gbL_4_3 = new QVBoxLayout;
    gbL_4_3->setAlignment(Qt::AlignCenter);
    QGroupBox *gb_4_3 = new QGroupBox(tr(">>> Start Calculate <<<"));
    gb_4_3->setAlignment(Qt::AlignCenter);
    gb_4_3->setMinimumHeight(500);
    gb_4_3->setLayout(gbL_4_3);

    QHBoxLayout *btnL_4_2 = new QHBoxLayout;
    QToolButton *btn_4_2 = new QToolButton;
    btn_4_2->setMinimumSize(QSize(192, 192));
    btn_4_2->setStyleSheet("QToolButton {background-color:white;border-radius:96px;}");
    btn_4_2->setIconSize(QSize(128, 128));
    btn_4_2->setIcon(QIcon(":/icon/logo.png"));
    btn_4_2->setText(tr("START\nCALCULATE"));
    QLabel *label_4_2 = new QLabel("[Log]");
    QProgressBar *pgb_4_1 = new QProgressBar;
    pgb_4_1->setAlignment(Qt::AlignCenter);

    gbL_4_3->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    btnL_4_2->addWidget(btn_4_2);
    gbL_4_3->addLayout(btnL_4_2);
    gbL_4_3->addWidget(label_4_2);
    gbL_4_3->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    gbL_4_3->addWidget(pgb_4_1);
    l_calc_4->addWidget(gb_4_3);

    // Apply Widget
    content_layout->addWidget(w_calc_4);
    group_calc.append(w_calc_4);

    // Make Connections
    connect(btn_4_2, &QToolButton::clicked, this, [=](){
        ui->scrollArea->verticalScrollBar()->setValue(0);
    });

    // ********** Part 5 **********

    // Build Widget
    QGridLayout *l_calc_5 = new QGridLayout;
    QWidget *w_calc_5 = new QWidget;
    w_calc_5->setLayout(l_calc_5);
    w_calc_5->hide();

    // Result & Export
    QGridLayout *gbL_5_1 = new QGridLayout;
    QGroupBox *gb_5_1 = new QGroupBox(tr("Result && Export"));
    gb_5_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    gb_5_1->setLayout(gbL_5_1);

    QComboBox *cbb_5_1 = new QComboBox;
    QToolButton *btn_5_1 = new QToolButton;
    btn_5_1->setText(tr("Export to file"));
    QLabel *label_5_1 = new QLabel("[Summary]\n");
    QListWidget *result_list_widget = new QListWidget;
    result_list_widget->setMinimumHeight(500);
    if (output_flows.size() > 0)
    {
        for (int i = 1; i <= output_flows.size(); i++)
            cbb_5_1->addItem(QString::number(i));

        for (int i = 1; i <= output_flows.size(); i++)
        {
            QHBoxLayout *layout = new QHBoxLayout;
            QWidget *item_widget = new QWidget;
            item_widget->setLayout(layout);

            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(item_widget->sizeHint());
            result_list_widget->addItem(item);
            result_list_widget->setItemWidget(item, item_widget);
        }
    }
    else
    {
        cbb_5_1->addItem(tr("[No Result]"));
    }

    gbL_5_1->addWidget(cbb_5_1, 0, 0, 1, 2);
    gbL_5_1->addWidget(btn_5_1, 0, 2, 1, 1);
    gbL_5_1->addWidget(label_5_1, 1, 0, 1, 3);
    gbL_5_1->addWidget(result_list_widget, 2, 0, 1, 3);
    l_calc_5->addWidget(gb_5_1);

    // Apply Widget
    content_layout->addWidget(w_calc_5);
    group_calc.append(w_calc_5);

    // ********** Apply Widget Group **********
    ui_item_set.append(group_calc);

    // ------------ Table Tab ------------
    QList<QWidget *> group_table;

    // ********** Part 1 **********

    // Build Widget
    QGridLayout *l_table_1 = new QGridLayout;
    QWidget *w_table_1 = new QWidget;
    w_table_1->setLayout(l_table_1);
    w_table_1->hide();

    QGroupBox *table_gb_1_1 = new QGroupBox("Table List");

    l_table_1->addWidget(table_gb_1_1);

    // Apply Widget
    content_layout->addWidget(w_table_1);
    group_table.append(w_table_1);

    // ********** Apply Widget Group **********
    ui_item_set.append(group_table);

    // ------------ Tool Tab ------------

    // ------------ Configuration Tab ------------

}

bool Graphics::activate(int index)
{
    if (0 <= index && index <= 3)
    {
        switch (index)
        {
        case 0:
            ui->listWidget->addItem(tr("[WELCOME]"));
            ui->listWidget->addItem(tr("Basic Section"));
            ui->listWidget->addItem(tr("Enchantment List"));
            ui->listWidget->addItem(tr("Item Pool (Advanced)"));
            ui->listWidget->addItem(tr("Algorithm & ALG Config"));
            ui->listWidget->addItem(tr("Result & Export"));

            ui->label_text->setText(tr("CALC."));
            break;
        case 1:
            ui->listWidget->addItem(tr("Table Manager"));
            ui->listWidget->addItem(tr("Basic Infomation"));
            ui->listWidget->addItem(tr("Edit Table"));
            ui->listWidget->addItem(tr("Create Table"));
            ui->listWidget->addItem(tr("Combine Table"));

            ui->label_text->setText(tr("TABLE"));
            break;
        case 2:
            ui->listWidget->addItem(tr("Test Cost"));
            ui->listWidget->addItem(tr("Test Penalty"));
            ui->listWidget->addItem(tr("Test Repairing"));
            ui->listWidget->addItem(tr("Test Multiplier"));

            ui->label_text->setText(tr("TOOL"));
            break;
        case 3:
            ui->listWidget->addItem(tr("Default"));
            ui->listWidget->addItem(tr("Lever"));
            ui->listWidget->addItem(tr("Update"));
            ui->listWidget->addItem(tr("About"));

            ui->label_text->setText(tr("CONF."));
            break;
        }

        if(index < ui_item_set.size())
        {
            for (int i = 0; i < ui_item_set[index].size(); i++)
                ui_item_set[index][i]->show();
        }

        return true;
    }
    return false;
}

bool Graphics::inactivate(int index)
{    
    if(0 <= index && index <= 3)
    {
        stored_pos[index] = ui->scrollArea->verticalScrollBar()->sliderPosition();
        qDebug() << "Stored Pos [W]:" << index << stored_pos[index];
        if(index < ui_item_set.size())
        {
            for (int i = 0; i < ui_item_set[index].size(); i++)
                ui_item_set[index][i]->hide();
        }

        return true;
    }
    return false;
}

void Graphics::switchTab(int n)
{
    if (n == current_widget)
        return;

    if(inactivate(current_widget))
    {
        ui->listWidget->clear();
        ui->label_text->setText("");
    }
    if (activate(n))
    {
        current_widget = n;
        emit currentTabChanged();
    }
}
