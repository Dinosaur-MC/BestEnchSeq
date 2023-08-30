#include "graphics.h"
#include "ui_graphics.h"
#include "core/tablemanager.h"
#include "langs/languagemgr.h"
#include "updatechecker.h"

#include <QApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTimer>
#include <QDir>

Graphics::Graphics(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::Graphics)
{
    // Load Table
    global_table_mgr.scanTable();                                           // Scan tables
    table_info_list = global_table_mgr.getAllTabeInfo();                    // Get table list
    int p = global_table_mgr.findTable(global_settings.last_used_table);    // find last used table
    if (p != -1)                                                            // Try to laod last used table, or load the internal table
        global_table_mgr.setCursor(p);
    else
        global_table_mgr.setCursor(0);
    global_table_mgr.apply();   // Apply table

    // Load Algorithm
    algorithm_list = alg_manager.getInternalAlgorithm();  // 载入内置算法
    algorithm_list.append(alg_manager.detectAlgorithm()); // 探测外部算法

    // Initialize Variable
    current_widget = -1;
    memset(stored_pos, 0, 4 * sizeof(int));

    all_enchantments.name = tr("All Enchantments");
    all_enchantments.icon_path = ":/icon/bookshelf.webp";
    selected_algorithm = global_settings.algorithm;

    // Setup UI
    ui->setupUi(this);
    this->setWindowTitle(TEXT_PROGRAM_NAME);

    setupPatterns();

    // Turn to default tab
    switchTab(0);
}

Graphics::~Graphics()
{
    global_settings.last_used_table = selected_table.file_name;
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

    connect(ui->btn_app_icon, &QToolButton::clicked, this, [=]()
    {
        QMessageBox notice(QMessageBox::Icon::Information, tr("Opening URL"), tr("Are you sure to open this URL?\n") + LINK_HOME_PAGE);
        notice.addButton(QMessageBox::Open);
        notice.addButton(QMessageBox::Cancel);
        if(notice.exec() == QMessageBox::Open)
            QDesktopServices::openUrl(QUrl(LINK_HOME_PAGE));
    });
    connect(ui->btn_calc, &QToolButton::clicked, this, [=]()
    {
        switchTab(0);
    });
    connect(ui->btn_table, &QToolButton::clicked, this, [=]()
    {
        switchTab(1);
    });
    connect(ui->btn_tool, &QToolButton::clicked, this, [=]()
    {
        switchTab(2);
    });
    connect(ui->btn_conf, &QToolButton::clicked, this, [=]()
    {
        switchTab(3);
    });

    // Sidebar & Content
    ui->label_text->setAlignment(Qt::AlignTop);

    QVBoxLayout *content_layout = new QVBoxLayout;
    content_layout->setAlignment(Qt::AlignTop);
    ui->scrollArea->widget()->setLayout(content_layout);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QTimer *timer = new QTimer(this);   // delay timer
    timer->setSingleShot(true);
    connect(this, &Graphics::currentTabChanged, this, [=](){ timer->start(1); });
    connect(timer, &QTimer::timeout, this, [=](){ ui->scrollArea->verticalScrollBar()->setValue(stored_pos[current_widget]); });

    setupTabCalc();
    setupTabTable();
    setupTabTool();
    setupTabConf();
}

void Graphics::setupTabCalc()
{
    // ------------ Calculate Tab ------------
    QList<QWidget *> group_calc;

    // ********** Part 1 **********
    {
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
        QRadioButton *radio_mce_bedrock = new QRadioButton(tr("Bedrock"));
        if (global_settings.edition == MCE::Java)
            radio_mce_java->setChecked(true);
        else if (global_settings.edition == MCE::Bedrock)
            radio_mce_bedrock->setChecked(true);
        selected_edition = global_settings.edition;

        gbL_1_1->addWidget(radio_mce_java);
        gbL_1_1->addWidget(radio_mce_bedrock);
        l_calc_1->addWidget(gb_1_1, 0, 0, 1, 4);

        // Table Selector
        QHBoxLayout *gbL_1_2 = new QHBoxLayout;
        QGroupBox *gb_1_2 = new QGroupBox(tr("Select Table"));
        gb_1_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_2->setLayout(gbL_1_2);

        QComboBox *cbb_1_1 = new QComboBox;
        cbb_1_1->setIconSize(QSize(48, 48));
        for (const auto &info: table_info_list)
        {
            if (info.enabled)
                cbb_1_1->addItem(info.file_name.endsWith(".json") ? QIcon(":/icon/json.svg") : QIcon(":/icon/csv.svg"), info.file_name);
        }
        selected_table = CTI;
        int p = cbb_1_1->findText(selected_table.file_name);
        if (p != -1)
            cbb_1_1->setCurrentIndex(p);
        else
        {
            cbb_1_1->setCurrentIndex(0);
            selected_table = table_info_list.at(0);
        }

        gbL_1_2->addWidget(cbb_1_1);
        l_calc_1->addWidget(gb_1_2, 1, 0, 1, 2);

        // Group Selector
        QHBoxLayout *gbL_1_3 = new QHBoxLayout;
        QGroupBox *gb_1_3 = new QGroupBox(tr("Select Group"));
        gb_1_3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_3->setLayout(gbL_1_3);

        QComboBox *cbb_1_2 = new QComboBox;
        cbb_1_2->setIconSize(QSize(48, 48));
        all_enchantments.enchantments = CT.enchs;
        cbb_1_2->addItem(QIcon(all_enchantments.icon_path), all_enchantments.name);
        for (const auto &group: CTG)
            cbb_1_2->addItem(QIcon(group.icon_path), group.name);
        cbb_1_2->setCurrentIndex(0);
        updateCurrentGroup(cbb_1_2->currentText());

        gbL_1_3->addWidget(cbb_1_2);
        l_calc_1->addWidget(gb_1_3, 1, 2, 1, 2);

        // ICM Selector
        QHBoxLayout *gbL_1_4 = new QHBoxLayout;
        QGroupBox *gb_1_4 = new QGroupBox(tr("Select Item Configuring Method"));
        gb_1_4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_4->setLayout(gbL_1_4);

        QRadioButton *radio_icm_normal = new QRadioButton(tr("Normal"));
        QRadioButton *radio_icm_poor = new QRadioButton(tr("Poor"));
        QRadioButton *radio_icm_advanced = new QRadioButton(tr("Advanced"));
        if (global_settings.item_config == ICM::Normal)
            radio_icm_normal->setChecked(true);
        else if (global_settings.item_config == ICM::Poor)
            radio_icm_poor->setChecked(true);
        else
            radio_icm_advanced->setChecked(true);
        selected_itemconfig = global_settings.item_config;

        gbL_1_4->addWidget(radio_icm_normal);
        gbL_1_4->addWidget(radio_icm_poor);
        gbL_1_4->addWidget(radio_icm_advanced);
        l_calc_1->addWidget(gb_1_4, 2, 0, 1, 2);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_calc_1);
        group_calc.append(w_calc_1);

        // ********** Make Connections **********

        // Edition
        connect(radio_mce_java, &QRadioButton::clicked, this, [=](bool checked){
            if (checked && selected_edition != MCE::Java)
            {
                selected_edition = MCE::Java;
                emit currentEditionChanged(MCE::Java);
            }
        });
        connect(radio_mce_bedrock, &QRadioButton::clicked, this, [=](bool checked){
            if (checked && selected_edition != MCE::Bedrock)
            {
                selected_edition = MCE::Bedrock;
                emit currentEditionChanged(MCE::Bedrock);
            }
        });

        // Table
        connect(this, &Graphics::tableListChanged, cbb_1_1, [=](){
            global_settings.last_used_table = cbb_1_1->currentText();

            cbb_1_1->blockSignals(true);
            cbb_1_1->clear();
            for (const auto &info: table_info_list)
            {
                if (info.enabled)
                    cbb_1_1->addItem(info.file_name.endsWith(".json") ? QIcon(":/icon/json.svg") : QIcon(":/icon/csv.svg"), info.file_name);
            }

            int p = cbb_1_1->findText(global_settings.last_used_table);
            if (p != -1)
                cbb_1_1->setCurrentIndex(p);
            else
                cbb_1_1->setCurrentIndex(0);

            cbb_1_1->blockSignals(false);
            emit currentTableChanged(cbb_1_1->currentIndex());
        });
        connect(cbb_1_1, &QComboBox::currentIndexChanged, this, [=](int p){
            selected_table = table_info_list.at(p);
            global_table_mgr.setCursor(p);
            global_table_mgr.apply();
            emit currentTableChanged(p);
        });

        // Group
        connect(this, &Graphics::currentTableChanged, cbb_1_2, [=](){
            all_enchantments.enchantments = CT.enchs;

            cbb_1_2->blockSignals(true);

            cbb_1_2->clear();
            cbb_1_2->addItem(QIcon(all_enchantments.icon_path), all_enchantments.name);
            for (const auto &group: CTG)
                cbb_1_2->addItem(QIcon(group.icon_path), group.name);
            cbb_1_2->setCurrentIndex(0);

            cbb_1_2->blockSignals(false);
            updateCurrentGroup(cbb_1_2->currentText());
            emit currentGroupChanged(cbb_1_2->currentText());
        });
        connect(cbb_1_2, &QComboBox::currentTextChanged, this, [=](const QString &name){
            updateCurrentGroup(name);
            emit currentGroupChanged(name);
        });

        // Item Config
        connect(radio_icm_normal, &QRadioButton::clicked, this, [=](bool checked){
            if(checked && selected_itemconfig != ICM::Normal)
            {
                selected_itemconfig = ICM::Normal;
                emit currentItemConfgChanged(ICM::Normal);
            }
        });
        connect(radio_icm_poor, &QRadioButton::clicked, this, [=](bool checked){
            if(checked && selected_itemconfig != ICM::Poor)
            {
                selected_itemconfig = ICM::Poor;
                emit currentItemConfgChanged(ICM::Poor);
            }
        });
        connect(radio_icm_advanced, &QRadioButton::clicked, this, [=](bool checked){
            if(checked && selected_itemconfig != ICM::Advanced)
            {
                selected_itemconfig = ICM::Advanced;
                emit currentItemConfgChanged(ICM::Advanced);
            }
        });
    }

    // ********** Part 2 **********
    {
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

        EnchListWidget *original_ench_list_widget = new EnchListWidget;
        original_ench_list_widget->setMinimumHeight(360);
        original_ench_list_widget->setEnchData(current_group.enchantments);
        original_enchantments = original_ench_list_widget->getEnchList();

        gbL_2_1->addWidget(original_ench_list_widget);
        l_calc_2->addWidget(gb_2_1);

        // Required Enchantment List
        QHBoxLayout *gbL_2_2 = new QHBoxLayout;
        QGroupBox *gb_2_2 = new QGroupBox(tr("Choose Required Enchantments"));
        gb_2_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_2_2->setLayout(gbL_2_2);

        EnchListWidget *required_ench_list_widget = new EnchListWidget;
        EnchDataList tm_ench_data_list = current_group.enchantments;
        QList<int> min_lvls(tm_ench_data_list.size());
        for (const Ench &e: original_enchantments)
        {
            for (int i = 0; i < tm_ench_data_list.size(); i++)
            {
                if (tm_ench_data_list.at(i).name == e.name)
                {
                    if (e.lvl < tm_ench_data_list.at(i).max_lvl)
                        min_lvls[i] = e.lvl + 1;
                    else
                    {
                        tm_ench_data_list.removeAt(i);
                        min_lvls.removeAt(i);
                        i--;
                    }
                }
                else if (tm_ench_data_list.at(i).conflictions.contains(e.name))
                {
                    tm_ench_data_list.removeAt(i);
                    min_lvls.removeAt(i);
                    i--;
                }
            }
        }
        required_ench_list_widget->setEnchData(tm_ench_data_list, min_lvls);
        required_ench_list_widget->setMinimumHeight(360);

        gbL_2_2->addWidget(required_ench_list_widget);
        l_calc_2->addWidget(gb_2_2);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_calc_2);
        group_calc.append(w_calc_2);

        // ********** Make Connections **********

        // Origin Enchantment List
        connect(this, &Graphics::currentGroupChanged, original_ench_list_widget, [=](){
            if (selected_itemconfig != ICM::Advanced)
                original_ench_list_widget->setEnchData(current_group.enchantments);
        });
        connect(this, &Graphics::currentItemConfgChanged, original_ench_list_widget, [=](ICM icm){
            if (icm == ICM::Advanced)
            {
                gb_2_1->hide();
                original_ench_list_widget->clear();
                gb_2_1->setEnabled(false);
            }
            else
            {
                gb_2_1->setEnabled(true);
                original_ench_list_widget->setEnchData(current_group.enchantments);
                gb_2_1->show();
            }
        });
        connect(original_ench_list_widget, &EnchListWidget::listDataChanged, this, [=](){
            original_enchantments = original_ench_list_widget->getEnchList();
        });
        connect(original_ench_list_widget, &EnchListWidget::itemValueChanged, this, [=](int index, int value){
            const EnchListItemWidget *w = (EnchListItemWidget *)original_ench_list_widget->itemWidget(original_ench_list_widget->item(index));
            int p = original_enchantments.indexOf(w->ench());
            if (p != -1)
                original_enchantments[p].lvl = value;
        });

        // Required Enchantment List
        connect(this, &Graphics::currentGroupChanged, required_ench_list_widget, [=](){
            if (selected_itemconfig == ICM::Advanced)
                required_ench_list_widget->setEnchData(current_group.enchantments);
        });
        connect(original_ench_list_widget, &EnchListWidget::listDataChanged, required_ench_list_widget, [=](){
            if (selected_itemconfig != ICM::Advanced)
            {
                EnchDataList tm_ench_data_list = current_group.enchantments;
                QList<int> min_lvls(tm_ench_data_list.size());
                for (const Ench &e: original_enchantments)
                {
                    for (int i = 0; i < tm_ench_data_list.size(); i++)
                    {
                        if (tm_ench_data_list.at(i).name == e.name)
                        {
                            if (e.lvl < tm_ench_data_list.at(i).max_lvl)
                                min_lvls[i] = e.lvl + 1;
                            else
                            {
                                tm_ench_data_list.removeAt(i);
                                min_lvls.removeAt(i);
                                i--;
                            }
                        }
                        else if (tm_ench_data_list.at(i).conflictions.contains(e.name))
                        {
                            tm_ench_data_list.removeAt(i);
                            min_lvls.removeAt(i);
                            i--;
                        }
                    }
                }
                required_ench_list_widget->setEnchData(tm_ench_data_list, min_lvls);
            }
        });
        connect(required_ench_list_widget, &EnchListWidget::listDataChanged, this, [=](){
            required_enchantments = required_ench_list_widget->getEnchList();
        });
        connect(required_ench_list_widget, &EnchListWidget::itemValueChanged, this, [=](int index, int value){
            const EnchListItemWidget *w = (EnchListItemWidget *)required_ench_list_widget->itemWidget(required_ench_list_widget->item(index));
            int p = required_enchantments.indexOf(w->ench());
            if (p != -1)
                required_enchantments[p].lvl = value;
        });
    }

    // ********** Part 3 **********
    {
        // Build Widget
        QGridLayout *l_calc_3 = new QGridLayout;
        QWidget *w_calc_3 = new QWidget;
        w_calc_3->setLayout(l_calc_3);
        w_calc_3->hide();
        w_calc_3->setDisabled(true);

        // Item Pool
        QHBoxLayout *gbL_3_1 = new QHBoxLayout;
        QGroupBox *gb_3_1 = new QGroupBox(tr("Edit Item Pool"));
        gb_3_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_3_1->setLayout(gbL_3_1);

        QListWidget *item_pool_list_widget = new QListWidget;
        item_pool_list_widget->setMinimumHeight(360);
        item_pool_list_widget->addItem(tr("(Click To Add Item)"));

        gbL_3_1->addWidget(item_pool_list_widget);
        l_calc_3->addWidget(gb_3_1);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_calc_3);
        group_calc.append(w_calc_3);

        // Make Connections
        connect(this, &Graphics::currentItemConfgChanged, item_pool_list_widget, [=](ICM icm){
            if (icm == ICM::Advanced && w_calc_3->isHidden())
            {
                w_calc_3->setEnabled(true);
                w_calc_3->show();
            }
            else
            {
                w_calc_3->hide();
                w_calc_3->setEnabled(false);
            }
        });
    }

    // ********** Part 4 **********
    {
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
        btn_4_1->setToolTip(tr("Refresh"));
        btn_4_1->setIconSize(QSize(20, 20));
        btn_4_1->setIcon(QIcon(":/icon/shuaxin.svg"));
        QComboBox *cbb_4_1 = new QComboBox;
        for (const auto &item: algorithm_list)
            cbb_4_1->addItem(item.name);
        selected_algorithm = algorithm_list.at(0).name;
        QLabel *label_4_1 = new QLabel;
        label_4_1->setOpenExternalLinks(true);
        for (const auto &item: algorithm_list)
        {
            if (selected_algorithm == item.name)
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
        gb_4_3->setMinimumHeight(360);
        gb_4_3->setLayout(gbL_4_3);

        QHBoxLayout *btnL_4_2 = new QHBoxLayout;
        QToolButton *btn_4_2 = new QToolButton;
        btn_4_2->setStyleSheet("QToolButton {background-color:white;border-radius:96px;}");
        btn_4_2->setMinimumSize(QSize(192, 192));
        btn_4_2->setToolTip(tr("Start Calculate"));
        btn_4_2->setIconSize(QSize(128, 128));
        btn_4_2->setIcon(QIcon(":/icon/logo.png"));
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
        ui->scrollArea->widget()->layout()->addWidget(w_calc_4);
        group_calc.append(w_calc_4);

        // Make Connections

    }

    // ********** Part 5 **********
    {
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
        QToolButton *btn_5_2 = new QToolButton;
        btn_5_2->setText(tr("Back To The Top"));

        gbL_5_1->addWidget(cbb_5_1, 0, 0, 1, 2);
        gbL_5_1->addWidget(btn_5_1, 0, 2, 1, 1);
        gbL_5_1->addWidget(label_5_1, 1, 0, 1, 3);
        gbL_5_1->addWidget(result_list_widget, 2, 0, 1, 3);
        gbL_5_1->addWidget(btn_5_2, 3, 2);
        l_calc_5->addWidget(gb_5_1);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_calc_5);
        group_calc.append(w_calc_5);

        // Make Connections
        connect(btn_5_2, &QToolButton::clicked, this, [=](){ ui->scrollArea->verticalScrollBar()->setValue(0); });
    }

    // ********** Apply Widget Group **********
    ui_item_set.append(group_calc);
}

void Graphics::setupTabTable()
{
    // ------------ Table Tab ------------
    QList<QWidget *> group_table;

    // ********** Part 1 **********
    {
        // Build Widget
        QGridLayout *l_table_1 = new QGridLayout;
        QWidget *w_table_1 = new QWidget;
        w_table_1->setLayout(l_table_1);
        w_table_1->hide();

        // Manage table
        QGridLayout *gbL_1_1 = new QGridLayout;
        QGroupBox *gb_1_1 = new QGroupBox(tr("Manage Tables"));
        gb_1_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_1->setLayout(gbL_1_1);

        QToolButton *btn_1_1 = new QToolButton;
        btn_1_1->setText(tr("Scan"));
        QToolButton *btn_1_2 = new QToolButton;
        btn_1_2->setText(tr("Load"));
        QToolButton *btn_1_3 = new QToolButton;
        btn_1_3->setText(tr("Enable"));
        QToolButton *btn_1_4 = new QToolButton;
        btn_1_4->setText(tr("Disable"));
        QToolButton *btn_1_5 = new QToolButton;
        btn_1_5->setText(tr("Import"));
        QToolButton *btn_1_6 = new QToolButton;
        btn_1_6->setText(tr("Delete"));
        QListWidget *lw_1_1 = new QListWidget;
        lw_1_1->setMinimumHeight(200);
        for (const auto &info: table_info_list)
        {
            lw_1_1->addItem(info.file_name);
            lw_1_1->item(lw_1_1->count()-1)->setIcon(info.file_name.endsWith(".json") ? QIcon(":/icon/json.svg") : QIcon(":/icon/csv.svg"));
        }
        QLabel *label_1_1 = new QLabel(tr("[Table Info]"));
        label_1_1->setAlignment(Qt::AlignTop);

        gbL_1_1->addWidget(btn_1_1, 0, 0, 1, 1);
        gbL_1_1->addWidget(btn_1_2, 0, 1, 1, 1);
        gbL_1_1->addWidget(btn_1_3, 0, 2, 1, 1);
        gbL_1_1->addWidget(btn_1_4, 0, 3, 1, 1);
        gbL_1_1->addWidget(btn_1_5, 0, 4, 1, 1);
        gbL_1_1->addWidget(btn_1_6, 0, 5, 1, 1);
        gbL_1_1->addWidget(lw_1_1, 1, 0, 10, 6);
        gbL_1_1->addWidget(label_1_1, 0, 6, 11, 3);
        l_table_1->addWidget(gb_1_1);

        // Current Table
        QGridLayout *gbL_1_2 = new QGridLayout;
        QGroupBox *gb_1_2 = new QGroupBox(tr("Current Table Data"));
        gb_1_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_2->setLayout(gbL_1_2);

        QLabel *label_1_2 = new QLabel(tr("Version:"));
        QLineEdit *le_1_1 = new QLineEdit;
        QLabel *label_1_3 = new QLabel(tr("Description:"));
        label_1_3->setAlignment(Qt::AlignTop);
        QTextEdit *te_1_1 = new QTextEdit;

        QToolButton *btn_1_7 = new QToolButton;
        btn_1_7->setMinimumSize(QSize(40, 40));
        btn_1_7->setToolTip(tr("Add enchantment"));
        btn_1_7->setText("E+");
        QToolButton *btn_1_8 = new QToolButton;
        btn_1_8->setMinimumSize(QSize(40, 40));
        btn_1_8->setToolTip(tr("Delete enchantment"));
        btn_1_8->setText("E-");
        QToolButton *btn_1_9 = new QToolButton;
        btn_1_9->setMinimumSize(QSize(40, 40));
        btn_1_9->setToolTip(tr("Add group"));
        btn_1_9->setText("G+");
        QToolButton *btn_1_10 = new QToolButton;
        btn_1_10->setMinimumSize(QSize(40, 40));
        btn_1_10->setToolTip(tr("Remove group"));
        btn_1_10->setText("G-");
        QToolButton *btn_1_11 = new QToolButton;
        btn_1_11->setMinimumSize(QSize(40, 40));
        btn_1_11->setToolTip(tr("Save change"));
        btn_1_11->setText("√");
        QToolButton *btn_1_12 = new QToolButton;
        btn_1_12->setMinimumSize(QSize(40, 40));
        btn_1_12->setToolTip(tr("Cancel change"));
        btn_1_12->setText("×");
        QTreeWidget *tw_1_1 = new QTreeWidget;
        tw_1_1->setHeaderLabels(QStringList() << tr("Name") << tr("Data"));
        tw_1_1->setMinimumHeight(480);
        tw_1_1->setColumnWidth(0, 180);
        tw_1_1->setColumnWidth(1, 420);
        QTreeWidgetItem *twitem_ench_1_1 = new QTreeWidgetItem({tr("Enchantments")});
        for (const auto &ench: current_table.enchs)
        {
            QTreeWidgetItem *e = new QTreeWidgetItem({ench.name});
            twitem_ench_1_1->addChild(e);
        }
        tw_1_1->addTopLevelItem(twitem_ench_1_1);
        QTreeWidgetItem *twitem_group_1_1 = new QTreeWidgetItem({tr("Groups")});
        for (const auto &group: current_table.groups)
        {
            QTreeWidgetItem *g = new QTreeWidgetItem({group.name});
            twitem_group_1_1->addChild(g);
        }
        tw_1_1->addTopLevelItem(twitem_group_1_1);

        gbL_1_2->addWidget(label_1_2, 0, 1, 1, 2);
        gbL_1_2->addWidget(le_1_1, 0, 2, 1, 4);
        gbL_1_2->addWidget(label_1_3, 1, 1, 1, 2);
        gbL_1_2->addWidget(te_1_1, 1, 2, 1, 4);
        gbL_1_2->addWidget(btn_1_7, 3, 0, 1, 1);
        gbL_1_2->addWidget(btn_1_8, 4, 0, 1, 1);
        gbL_1_2->addWidget(btn_1_9, 5, 0, 1, 1);
        gbL_1_2->addWidget(btn_1_10, 6, 0, 1, 1);
        gbL_1_2->addWidget(btn_1_11, 7, 0, 1, 1);
        gbL_1_2->addWidget(btn_1_12, 8, 0, 1, 1);
        gbL_1_2->addWidget(tw_1_1, 3, 1, 19, 5);
        l_table_1->addWidget(gb_1_2);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_table_1);
        group_table.append(w_table_1);
    }

    // ********** Part 2 **********
    {
        // Build Widget
        QGridLayout *l_table_2 = new QGridLayout;
        QWidget *w_table_2 = new QWidget;
        w_table_2->setLayout(l_table_2);
        w_table_2->hide();

        // New Table
        QHBoxLayout *gbL_2_1 = new QHBoxLayout;
        QGroupBox *gb_2_1 = new QGroupBox(tr("New Table"));
        gb_2_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_2_1->setAlignment(Qt::AlignCenter);
        gb_2_1->setLayout(gbL_2_1);

        QToolButton *btn_2_1 = new QToolButton;
        btn_2_1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn_2_1->setText(tr("JSON Format"));
        btn_2_1->setIconSize(QSize(48, 48));
        btn_2_1->setIcon(QIcon(":/icon/json.svg"));
        QToolButton *btn_2_2 = new QToolButton;
        btn_2_2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn_2_2->setText(tr("CSV Format"));
        btn_2_2->setIconSize(QSize(48, 48));
        btn_2_2->setIcon(QIcon(":/icon/csv.svg"));

        gbL_2_1->addWidget(btn_2_1);
        gbL_2_1->addWidget(btn_2_2);
        l_table_2->addWidget(gb_2_1);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_table_2);
        group_table.append(w_table_2);

        // Make Connections
        connect(btn_2_1, &QToolButton::clicked, this, [=](){
            QDialog dialog;
            dialog.setWindowTitle(tr("Create table in JSON format"));
            QVBoxLayout l0;
            dialog.setLayout(&l0);

            QLineEdit le;
            le.setFont(QFont("微软雅黑", 16));
            le.setPlaceholderText(tr("Table name"));

            QWidget w1, w2;
            QHBoxLayout l1, l2;
            l1.setAlignment(Qt::AlignLeft);
            l2.setAlignment(Qt::AlignRight);
            w1.setLayout(&l1);
            w2.setLayout(&l2);

            QCheckBox chkb("Template from");
            chkb.setFont(QFont("微软雅黑", 12));
            QComboBox cbb;
            cbb.setEnabled(false);
            cbb.setIconSize(QSize(48, 48));
            for (const auto &info: table_info_list)
                cbb.addItem(info.file_name.endsWith(".json") ? QIcon(":/icon/json.svg") : QIcon(":/icon/csv.svg"), info.file_name);
            QPushButton btn1(tr("Confirm"));
            QPushButton btn2(tr("Cancel"));

            l0.addWidget(&le);
            l0.addWidget(&w1);
            l0.addWidget(&w2);
            l1.addWidget(&chkb);
            l1.addWidget(&cbb);
            l2.addWidget(&btn1);
            l2.addWidget(&btn2);

            connect(&chkb, &QCheckBox::stateChanged, &cbb, [&cbb](){ cbb.setEnabled(!cbb.isEnabled()); });
            connect(&btn1, &QPushButton::clicked, &dialog, &QDialog::accept);
            connect(&btn2, &QPushButton::clicked, &dialog, &QDialog::reject);

            qDebug() << "[QDialog] exec";
            if (dialog.exec() == QDialog::Accepted)
            {
                qDebug() << "[QDialog] Accepted";
                QString name = le.text();          // new table name
                bool checked = chkb.isChecked();
                QString temp = cbb.currentText();  // template table name

                qDebug() << "[QDialog]" << name << checked << temp;
                /* Read the template table & copy to new one then open it */
            }
            else
                qDebug() << "[QDialog] Rejected";
        });
        connect(btn_2_2, &QToolButton::clicked, this, [=](){
            QDialog dialog;
            dialog.setWindowTitle(tr("Create table in CSV format"));
            QVBoxLayout l0;
            dialog.setLayout(&l0);

            QLineEdit le;
            le.setFont(QFont("微软雅黑", 16));
            le.setPlaceholderText(tr("Table name"));

            QWidget w1, w2;
            QHBoxLayout l1, l2;
            l1.setAlignment(Qt::AlignLeft);
            l2.setAlignment(Qt::AlignRight);
            w1.setLayout(&l1);
            w2.setLayout(&l2);

            QCheckBox chkb("Template from");
            chkb.setFont(QFont("微软雅黑", 12));
            QComboBox cbb;
            cbb.setEnabled(false);
            cbb.setIconSize(QSize(48, 48));
            for (const auto &info: table_info_list)
                cbb.addItem(info.file_name.endsWith(".json") ? QIcon(":/icon/json.svg") : QIcon(":/icon/csv.svg"), info.file_name);
            QPushButton btn1(tr("Confirm"));
            QPushButton btn2(tr("Cancel"));

            l0.addWidget(&le);
            l0.addWidget(&w1);
            l0.addWidget(&w2);
            l1.addWidget(&chkb);
            l1.addWidget(&cbb);
            l2.addWidget(&btn1);
            l2.addWidget(&btn2);

            connect(&chkb, &QCheckBox::stateChanged, &cbb, [&cbb](){ cbb.setEnabled(!cbb.isEnabled()); });
            connect(&btn1, &QPushButton::clicked, &dialog, &QDialog::accept);
            connect(&btn2, &QPushButton::clicked, &dialog, &QDialog::reject);

            qDebug() << "[QDialog] exec";
            if (dialog.exec() == QDialog::Accepted)
            {
                qDebug() << "[QDialog] Accepted";
                QString name = le.text();          // new table name
                bool checked = chkb.isChecked();
                QString temp = cbb.currentText();  // template table name

                qDebug() << "[QDialog]" << name << checked << temp;
                /* Read the template table & copy to new one then open it */
            }
            else
                qDebug() << "[QDialog] Rejected";
        });
    }

    // ********** Part 3 **********
    {
        // Build Widget
        QGridLayout *l_table_3 = new QGridLayout;
        QWidget *w_table_3 = new QWidget;
        w_table_3->setLayout(l_table_3);
        w_table_3->hide();

        // Merge Tables
        QGridLayout *gbL_3_1 = new QGridLayout;
        QGroupBox *gb_3_1 = new QGroupBox(tr("Merge Tables"));
        gb_3_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_3_1->setAlignment(Qt::AlignCenter);
        gb_3_1->setLayout(gbL_3_1);

        QComboBox *cbb_3_1 = new QComboBox;
        cbb_3_1->setIconSize(QSize(32, 32));
        for (const auto &info: table_info_list)
            cbb_3_1->addItem(info.file_name.endsWith(".json") ? QIcon(":/icon/json.svg") : QIcon(":/icon/csv.svg"), info.file_name);
        QComboBox *cbb_3_2 = new QComboBox;
        cbb_3_2->setIconSize(QSize(32, 32));
        for (const auto &info: table_info_list)
            cbb_3_2->addItem(info.file_name.endsWith(".json") ? QIcon(":/icon/json.svg") : QIcon(":/icon/csv.svg"), info.file_name);

        QTreeWidget *tw_3_1 = new QTreeWidget;
        tw_3_1->setMinimumHeight(480);
        tw_3_1->setHeaderLabels(QStringList() << tr("Name") << tr("Data"));
        tw_3_1->setColumnWidth(0, 180);
        tw_3_1->setColumnWidth(1, 420);
        QTreeWidgetItem *twitem_ench_3_1 = new QTreeWidgetItem({tr("Enchantments")});
        tw_3_1->addTopLevelItem(twitem_ench_3_1);
        QTreeWidgetItem *twitem_group_3_1 = new QTreeWidgetItem({tr("Groups")});
        tw_3_1->addTopLevelItem(twitem_group_3_1);
        QToolButton *btn_3_1 = new QToolButton;
        btn_3_1->setMinimumSize(QSize(40, 40));
        btn_3_1->setToolTip(tr("Compare"));
        btn_3_1->setText("C");
        QToolButton *btn_3_2 = new QToolButton;
        btn_3_2->setMinimumSize(QSize(40, 40));
        btn_3_2->setToolTip(tr("Cover left"));
        btn_3_2->setText("←");
        QToolButton *btn_3_3 = new QToolButton;
        btn_3_3->setMinimumSize(QSize(40, 40));
        btn_3_3->setToolTip(tr("Cover right"));
        btn_3_3->setText("→");
        QToolButton *btn_3_4 = new QToolButton;
        btn_3_4->setMinimumSize(QSize(40, 40));
        btn_3_4->setToolTip(tr("Fill blank"));
        btn_3_4->setText("A");
        QToolButton *btn_3_5 = new QToolButton;
        btn_3_5->setMinimumSize(QSize(40, 40));
        btn_3_5->setToolTip(tr("Save"));
        btn_3_5->setText("S");
        QTreeWidget *tw_3_2 = new QTreeWidget;
        tw_3_2->setMinimumHeight(480);
        tw_3_2->setHeaderLabels(QStringList() << tr("Name") << tr("Data"));
        tw_3_2->setColumnWidth(0, 180);
        tw_3_2->setColumnWidth(1, 420);
        QTreeWidgetItem *twitem_ench_3_2 = new QTreeWidgetItem({tr("Enchantments")});
        tw_3_2->addTopLevelItem(twitem_ench_3_2);
        QTreeWidgetItem *twitem_group_3_2 = new QTreeWidgetItem({tr("Groups")});
        tw_3_2->addTopLevelItem(twitem_group_3_2);

        gbL_3_1->addWidget(cbb_3_1, 0, 0, 1, 3);
        gbL_3_1->addWidget(cbb_3_2, 0, 4, 1, 3);
        gbL_3_1->addWidget(tw_3_1, 1, 0, 19, 3);
        gbL_3_1->addWidget(btn_3_1, 9, 3, 1, 1);
        gbL_3_1->addWidget(btn_3_2, 10, 3, 1, 1);
        gbL_3_1->addWidget(btn_3_3, 11, 3, 1, 1);
        gbL_3_1->addWidget(btn_3_4, 12, 3, 1, 1);
        gbL_3_1->addWidget(btn_3_5, 1, 3, 1, 1);
        gbL_3_1->addWidget(tw_3_2, 1, 4, 19, 3);
        l_table_3->addWidget(gb_3_1);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_table_3);
        group_table.append(w_table_3);
    }

    // ********** Apply Widget Group **********
    ui_item_set.append(group_table);
}

void Graphics::setupTabTool()
{
    // ------------ Tool Tab ------------
    QList<QWidget *> group_tool;

    // ********** Part 1 **********
    {
        // Build Widget
        QGridLayout *l_tool_1 = new QGridLayout;
        QWidget *w_tool_1 = new QWidget;
        w_tool_1->setLayout(l_tool_1);
        w_tool_1->hide();

        // Test Forge
        QVBoxLayout *gbL_1_1 = new QVBoxLayout;
        QGroupBox *gb_1_1 = new QGroupBox(tr("Test Forge"));
        gb_1_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_1->setAlignment(Qt::AlignCenter);
        gb_1_1->setLayout(gbL_1_1);

        QHBoxLayout *layout_1_1 = new QHBoxLayout;
        layout_1_1->setAlignment(Qt::AlignLeft);
        QComboBox *cbb_1_1 = new QComboBox;
        cbb_1_1->addItems(QStringList() << tr("Java") << tr("Bedrock"));
        QToolButton *btn_1_1 = new QToolButton;
        btn_1_1->setIconSize(QSize(48, 48));
        QLabel *label_1_1 = new QLabel("+");
        QToolButton *btn_1_2 = new QToolButton;
        btn_1_2->setIconSize(QSize(48, 48));
        QLabel *label_1_2 = new QLabel("=");
        QToolButton *btn_1_3 = new QToolButton;
        btn_1_3->setIconSize(QSize(48, 48));
        QLabel *label_1_3 = new QLabel("[COST]");
        QTreeWidget *tw_1_1 = new QTreeWidget;
        tw_1_1->setMinimumHeight(240);
        tw_1_1->setHeaderLabels(QStringList() << tr("Label") << tr("Item A") << tr("Item B") << tr("Output"));
        QComboBox *cbb_1_2 = new QComboBox;
        cbb_1_2->setIconSize(QSize(48, 48));
        for (const auto &info: table_info_list)
            cbb_1_2->addItem(info.file_name.endsWith(".json") ? QIcon(":/icon/json.svg") : QIcon(":/icon/csv.svg"), info.file_name);

        layout_1_1->addWidget(cbb_1_1);
        layout_1_1->addWidget(btn_1_1);
        layout_1_1->addWidget(label_1_1);
        layout_1_1->addWidget(btn_1_2);
        layout_1_1->addWidget(label_1_2);
        layout_1_1->addWidget(btn_1_3);
        layout_1_1->addWidget(label_1_3);
        layout_1_1->addStretch(1);
        layout_1_1->addWidget(cbb_1_2);
        gbL_1_1->addLayout(layout_1_1);
        gbL_1_1->addWidget(tw_1_1);
        l_tool_1->addWidget(gb_1_1);

        // Test Penalty
        QGridLayout *gbL_1_2 = new QGridLayout;
        QGroupBox *gb_1_2 = new QGroupBox(tr("Test Penalty"));
        gb_1_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_2->setAlignment(Qt::AlignCenter);
        gb_1_2->setLayout(gbL_1_2);

        QLabel *label_1_4 = new QLabel(tr("[Equation]\nCost: f(n) = 2^(n) - 1\n[Shorthand List]\n1 Penalty L =   1 EXP L\n2 Penalty L =   3 EXP L\n3 Penalty L =   7 EXP L\n4 Penalty L = 15 EXP L\n5 Penalty L = 31 EXP L\n6 Penalty L = 63 EXP L"));
        label_1_4->setAlignment(Qt::AlignCenter);
        QLabel *label_1_5 = new QLabel(tr("Penalty L to EXP L:"));
        QSpinBox *spb_1_1 = new QSpinBox;
        QLabel *label_1_6 = new QLabel("→");
        label_1_6->setAlignment(Qt::AlignCenter);
        QSpinBox *spb_1_2 = new QSpinBox;
        QLabel *label_1_7 = new QLabel(tr("EXP L to Penalty L:"));
        QSpinBox *spb_1_3 = new QSpinBox;
        QLabel *label_1_8 = new QLabel("→");
        label_1_8->setAlignment(Qt::AlignCenter);
        QSpinBox *spb_1_4 = new QSpinBox;

        gbL_1_2->addWidget(label_1_4, 0, 8, 9, 10);
        gbL_1_2->addWidget(label_1_5, 0, 0, 1, 3);
        gbL_1_2->addWidget(spb_1_1, 0, 3, 1, 1);
        gbL_1_2->addWidget(label_1_6, 0, 4, 1, 1);
        gbL_1_2->addWidget(spb_1_2, 0, 5, 1, 1);
        gbL_1_2->addWidget(label_1_7, 1, 0, 1, 3);
        gbL_1_2->addWidget(spb_1_3, 1, 3, 1, 1);
        gbL_1_2->addWidget(label_1_8, 1, 4, 1, 1);
        gbL_1_2->addWidget(spb_1_4, 1, 5, 1, 1);
        l_tool_1->addWidget(gb_1_2);

        // Test Multiplier
        QHBoxLayout *gbL_1_3 = new QHBoxLayout;
        QGroupBox *gb_1_3 = new QGroupBox(tr("Test Multiplier"));
        gb_1_3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_3->setAlignment(Qt::AlignCenter);
        gb_1_3->setLayout(gbL_1_3);

        QLabel *label_1_9 = new QLabel(tr("\n"));

        gbL_1_3->addWidget(label_1_9);
        l_tool_1->addWidget(gb_1_3);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_tool_1);
        group_tool.append(w_tool_1);
    }

    // ********** Apply Widget Group **********
    ui_item_set.append(group_tool);
}

void Graphics::setupTabConf()
{
    // ------------ Configuration Tab ------------
    QList<QWidget *> group_confg;

    // ********** Part 1 **********
    {
        // Build Widget
        QGridLayout *l_confg_1 = new QGridLayout;
        QWidget *w_confg_1 = new QWidget;
        w_confg_1->setLayout(l_confg_1);
        w_confg_1->hide();

        // Default Values
        QGridLayout *gbL_1_1 = new QGridLayout;
        QGroupBox *gb_1_1 = new QGroupBox(tr("Defaults"));
        gb_1_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_1->setAlignment(Qt::AlignCenter);
        gb_1_1->setLayout(gbL_1_1);

        QLabel *label_1_1 = new QLabel(tr("Edition:"));
        label_1_1->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QComboBox *cbb_1_1 = new QComboBox;
        cbb_1_1->addItem(tr("Java"));
        cbb_1_1->addItem(tr("Bedrock"));
        int p_cbb_1_1 = cbb_1_1->findText(MCEToString(global_settings.edition));
        if (p_cbb_1_1 != -1)
            cbb_1_1->setCurrentIndex(p_cbb_1_1);
        else
        {
            cbb_1_1->setCurrentIndex(0);
            global_settings.edition = StringToMCE(cbb_1_1->currentText());
        }
        QLabel *label_1_2 = new QLabel(tr("Item Config:"));
        label_1_2->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QComboBox *cbb_1_2 = new QComboBox;
        cbb_1_2->addItem(tr("Normal"));
        cbb_1_2->addItem(tr("Poor"));
        cbb_1_2->addItem(tr("Advanced"));
        int p_cbb_1_2 = cbb_1_2->findText(tr(ICMToString(global_settings.item_config).toUtf8().data()));
        if (p_cbb_1_2 != -1)
            cbb_1_2->setCurrentIndex(p_cbb_1_2);
        else
        {
            cbb_1_2->setCurrentIndex(0);
            global_settings.item_config = ICM::Normal;
        }
        QLabel *label_1_3 = new QLabel(tr("Algorithm:"));
        label_1_3->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QComboBox *cbb_1_3 = new QComboBox;
        for (const auto &item: algorithm_list)
            cbb_1_3->addItem(item.name);
        int p_cbb_1_3 = cbb_1_3->findText(global_settings.algorithm);
        if (p_cbb_1_3 != -1)
            cbb_1_3->setCurrentIndex(p_cbb_1_3);
        else
        {
            cbb_1_3->setCurrentIndex(0);
            global_settings.algorithm = cbb_1_3->currentText();
        }
        QLabel *label_1_4 = new QLabel(tr("Language:"));
        label_1_4->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QComboBox *cbb_1_4 = new QComboBox;
        const QStringList list = global_lang_mgr.langaugeNameList();
        for (const auto &name : list)
            cbb_1_4->addItem(name);
        int p_cbb_1_4 = global_lang_mgr.langaugeShortNameList().indexOf(global_settings.language);
        if (p_cbb_1_4 != -1)
            cbb_1_4->setCurrentIndex(p_cbb_1_4);
        else
        {
            cbb_1_4->setCurrentIndex(0);
            global_settings.language = global_lang_mgr.langaugeShortNameList().at(cbb_1_4->currentIndex());
        }
        QLabel *label_1_5 = new QLabel(tr("Export Path:"));
        label_1_5->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QLineEdit *le_1_1 = new QLineEdit;
        le_1_1->setText(QDir(global_settings.export_path).absolutePath());
        QToolButton *btn_1_1 = new QToolButton;
        btn_1_1->setToolTip(tr("Browse..."));
        btn_1_1->setIconSize(QSize(20, 20));
        btn_1_1->setIcon(QIcon(":/icon/editor.svg"));

        gbL_1_1->addWidget(label_1_1, 0, 0, 1, 1);
        gbL_1_1->addWidget(cbb_1_1, 0, 1, 1, 1);
        gbL_1_1->addWidget(label_1_2, 0, 3, 1, 1);
        gbL_1_1->addWidget(cbb_1_2, 0, 4, 1, 1);
        gbL_1_1->addWidget(label_1_3, 1, 0, 1, 1);
        gbL_1_1->addWidget(cbb_1_3, 1, 1, 1, 1);
        gbL_1_1->addWidget(label_1_4, 1, 3, 1, 1);
        gbL_1_1->addWidget(cbb_1_4, 1, 4, 1, 1);
        gbL_1_1->addWidget(label_1_5, 2, 0, 1, 1);
        gbL_1_1->addWidget(le_1_1, 2, 1, 1, 4);
        gbL_1_1->addWidget(btn_1_1, 2, 5, 1, 1);
        l_confg_1->addWidget(gb_1_1);

        // Levers
        QGridLayout *gbL_1_2 = new QGridLayout;
        QGroupBox *gb_1_2 = new QGroupBox(tr("Levers"));
        gb_1_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_1_2->setAlignment(Qt::AlignCenter);
        gb_1_2->setLayout(gbL_1_2);

        QCheckBox *chkb_1_1 = new QCheckBox(tr("Auto Save"));
        chkb_1_1->setChecked(global_settings.auto_save);
        QCheckBox *chkb_1_2 = new QCheckBox(tr("Auto Check Update"));
        chkb_1_2->setChecked(global_settings.auto_check_update);
        QCheckBox *chkb_1_3 = new QCheckBox(tr("Enable Lax Window Resizing"));
        chkb_1_3->setChecked(global_settings.enable_lax_window_resizing);

        gbL_1_2->addWidget(chkb_1_1);
        gbL_1_2->addWidget(chkb_1_2);
        gbL_1_2->addWidget(chkb_1_3);
        l_confg_1->addWidget(gb_1_2);

        // Operations
        QHBoxLayout *wL_1_1 = new QHBoxLayout;
        QWidget *w_1_1 = new QWidget;
        wL_1_1->setAlignment(Qt::AlignRight);
        w_1_1->setLayout(wL_1_1);

        QPushButton *btn_1_2 = new QPushButton(tr("Reset"));
        QPushButton *btn_1_3 = new QPushButton(tr("Cancel"));
        QPushButton *btn_1_4 = new QPushButton(tr("Save"));

        wL_1_1->addWidget(btn_1_2);
        wL_1_1->addWidget(btn_1_3);
        wL_1_1->addWidget(btn_1_4);
        l_confg_1->addWidget(w_1_1);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_confg_1);
        group_confg.append(w_confg_1);

        // Make Connections
        connect(le_1_1, &QLineEdit::textChanged, this, [=](){
            QPalette palette;
            palette.setColor(QPalette::Text, QColor("black"));
            le_1_1->setPalette(palette);
        });
        connect(btn_1_1, &QToolButton::clicked, this, [=](){
            QFileDialog file_dialog;
            file_dialog.setAcceptMode(QFileDialog::AcceptOpen);
            file_dialog.setFileMode(QFileDialog::Directory);
            file_dialog.setOption(QFileDialog::ShowDirsOnly);
            file_dialog.setDirectory(QDir(global_settings.export_path));
            file_dialog.exec();
            QString dir = file_dialog.selectedFiles().at(0);
            if (!dir.isEmpty())
            {
                le_1_1->blockSignals(true);
                le_1_1->setText(dir);
                le_1_1->blockSignals(false);
            }
        });
        connect(btn_1_2, &QPushButton::clicked, this, [=](){
            QMessageBox msgb(QMessageBox::Icon::Warning, tr("Reset Settings"), tr("Are you sure to reset the application settings?\nAll the configuration will set to default."));
            msgb.addButton(QMessageBox::Reset);
            msgb.addButton(QMessageBox::Cancel);
            if(msgb.exec() == QMessageBox::Reset)
            {
                global_settings = defaultSettings();
                QFile file(FILE_CONFIG);
                if (file.remove())
                    QApplication::exit(CODE_RESTART_WITHOUT_SAVING);
                else
                    QApplication::exit(0);
            }
        });
        connect(btn_1_3, &QPushButton::clicked, this, [=](){
            cbb_1_1->setCurrentText(MCEToString(global_settings.edition));
            cbb_1_2->setCurrentText(tr(ICMToString(global_settings.item_config).toUtf8().data()));
            cbb_1_3->setCurrentText(global_settings.algorithm);
            cbb_1_4->setCurrentText(global_lang_mgr.langaugeNameList().at(global_lang_mgr.langaugeShortNameList().indexOf(global_settings.language)));
            le_1_1->setText(QDir(global_settings.export_path).absolutePath());
            chkb_1_1->setChecked(global_settings.auto_save);
            chkb_1_2->setChecked(global_settings.auto_check_update);
            chkb_1_3->setChecked(global_settings.enable_lax_window_resizing);
        });
        connect(btn_1_4, &QPushButton::clicked, this, [=](){
            bool pass = true;

            QDir dir(le_1_1->text());
            if (!dir.exists())
            {
                QPalette palette;
                palette.setColor(QPalette::Text, QColor("red"));
                le_1_1->setPalette(palette);
                pass = false;
            }

            if (pass)
            {
                global_settings.edition = StringToMCE(cbb_1_1->currentText());
                global_settings.item_config = QList<ICM>({ICM::Normal, ICM::Poor, ICM::Advanced}).at(cbb_1_2->currentIndex());
                global_settings.algorithm = cbb_1_3->currentText();
                global_settings.language = global_lang_mgr.langaugeShortNameList().at(cbb_1_4->currentIndex());
                global_settings.export_path = le_1_1->text().replace('\\', '/');
                global_settings.auto_save = chkb_1_1->isChecked();
                global_settings.auto_check_update = chkb_1_2->isChecked();
                global_settings.enable_lax_window_resizing = chkb_1_3->isChecked();
                QApplication::exit(CODE_RESTART);
            }
        });

    }

    // ********** Part 2 **********
    {
        // Build Widget
        QGridLayout *l_confg_2 = new QGridLayout;
        QWidget *w_confg_2 = new QWidget;
        w_confg_2->setLayout(l_confg_2);
        w_confg_2->hide();

        // About
        QGridLayout *gbL_2_1 = new QGridLayout;
        QGroupBox *gb_2_1 = new QGroupBox(tr("About"));
        gb_2_1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        gb_2_1->setAlignment(Qt::AlignCenter);
        gb_2_1->setLayout(gbL_2_1);

        QLabel *label_2_1 = new QLabel(tr("Version:"));
        label_2_1->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QLabel *label_2_2 = new QLabel(QString(VERSION_NAME) + " (" + QString::number(VERSION_ID) + ")");
        QLabel *label_2_3 = new QLabel(tr("Author:"));
        label_2_3->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QLabel *label_2_4 = new QLabel(TEXT_AUTHOR);
        QLabel *label_2_5 = new QLabel(tr("License:"));
        label_2_5->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        QLabel *label_2_6 = new QLabel(LICENSE);
        QPushButton *btn_2_1 = new QPushButton(tr("Website"));
        QPushButton *btn_2_2 = new QPushButton(tr("Feedback"));
        QPushButton *btn_2_3 = new QPushButton(tr("Check Update"));
        QLabel *label_2_7 = new QLabel(tr(""));
        label_2_7->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        gbL_2_1->addWidget(label_2_1, 0, 0, 1, 1);
        gbL_2_1->addWidget(label_2_2, 0, 1, 1, 2);
        gbL_2_1->addWidget(label_2_3, 1, 0, 1, 1);
        gbL_2_1->addWidget(label_2_4, 1, 1, 1, 2);
        gbL_2_1->addWidget(label_2_5, 2, 0, 1, 1);
        gbL_2_1->addWidget(label_2_6, 2, 1, 1, 2);
        gbL_2_1->addWidget(btn_2_1, 0, 3, 1, 1);
        gbL_2_1->addWidget(btn_2_2, 1, 3, 1, 1);
        gbL_2_1->addWidget(btn_2_3, 2, 3, 1, 1);
        gbL_2_1->addWidget(label_2_7, 3, 0, 1, 4);
        l_confg_2->addWidget(gb_2_1);

        // Apply Widget
        ui->scrollArea->widget()->layout()->addWidget(w_confg_2);
        group_confg.append(w_confg_2);

        // Make Connections
        connect(btn_2_1, &QPushButton::clicked, this, [=](){
            QMessageBox notice(QMessageBox::Icon::Information, tr("Opening URL"), tr("Are you sure to open this URL?\n") + LINK_HOME_PAGE);
            notice.addButton(QMessageBox::Open);
            notice.addButton(QMessageBox::Cancel);
            if(notice.exec() == QMessageBox::Open)
                QDesktopServices::openUrl(QUrl(LINK_HOME_PAGE));
        });
        connect(btn_2_2, &QPushButton::clicked, this, [=](){
            QMessageBox notice(QMessageBox::Icon::Information, tr("Opening URL"), tr("Are you sure to open this URL?\n") + LINK_FEEDBACK_PAGE);
            notice.addButton(QMessageBox::Open);
            notice.addButton(QMessageBox::Cancel);
            if(notice.exec() == QMessageBox::Open)
                QDesktopServices::openUrl(QUrl(LINK_FEEDBACK_PAGE));
        });
        connect(btn_2_3, &QPushButton::clicked, this, [=](){
            btn_2_3->setEnabled(false);
            UpdateChecker *update = new UpdateChecker;
            connect(update, &UpdateChecker::finished, this, [=](){
                btn_2_3->setEnabled(true);
            });
            update->check(QUrl(LINK_UPDATE_DATA), true);
        });
    }

    // ********** Apply Widget Group **********
    ui_item_set.append(group_confg);
}

void Graphics::updateCurrentGroup(QString name)
{
    if (name == tr("All Enchantments"))
    {
        current_group = all_enchantments;
        for (int i = 0; i < current_group.enchantments.size(); i++)
        {
            if (!current_group.enchantments[i].editions.contains(selected_edition))
            {
                current_group.enchantments.removeAt(i);
                i--;
            }
        }
    }
    else
    {
        for (const auto &group: CTG)
        {
            if (group.name == name)
            {
                current_group = group;
                for (int i = 0; i < current_group.enchantments.size(); i++)
                {
                    if (!current_group.enchantments[i].editions.contains(selected_edition))
                    {
                        current_group.enchantments.removeAt(i);
                        i--;
                    }
                }
            }
        }
    }
}

bool Graphics::activate(int index)
{
    if (0 <= index && index <= 3)
    {
        switch (index)
        {
        case 0:
            ui->listWidget->addItem(tr("[WELCOME PAGE]"));
            ui->listWidget->addItem(tr("Basic Section"));
            ui->listWidget->addItem(tr("Origin Enchantment"));
            ui->listWidget->addItem(tr("Required Enchantment"));
            ui->listWidget->addItem(tr("Item Pool (Advanced)"));
            ui->listWidget->addItem(tr("Algorithm & ALG Config"));
            ui->listWidget->addItem(tr("Start Calculate"));
            ui->listWidget->addItem(tr("Result & Export"));

            ui->label_text->setText(tr("CALC."));
            break;
        case 1:
            ui->listWidget->addItem(tr("Manage Tables"));
            ui->listWidget->addItem(tr("Current Table"));
            ui->listWidget->addItem(tr("New Table"));
            ui->listWidget->addItem(tr("Merge Tables"));

            ui->label_text->setText(tr("TABLE"));
            break;
        case 2:
            ui->listWidget->addItem(tr("Test Forge"));
            ui->listWidget->addItem(tr("Test Penalty"));
            ui->listWidget->addItem(tr("Test Multiplier"));

            ui->label_text->setText(tr("TOOL"));
            break;
        case 3:
            ui->listWidget->addItem(tr("Default"));
            ui->listWidget->addItem(tr("Lever"));
            ui->listWidget->addItem(tr("About"));
            ui->listWidget->addItem(tr("Update"));

            ui->label_text->setText(tr("CONF."));
            break;
        }

        if (index < ui_item_set.size())
        {
            for (int i = 0; i < ui_item_set[index].size(); i++)
            {
                if (ui_item_set[index][i]->isEnabled())
                    ui_item_set[index][i]->show();
            }
        }

        return true;
    }
    return false;
}

bool Graphics::inactivate(int index)
{
    if (0 <= index && index <= 3)
    {
        stored_pos[index] = ui->scrollArea->verticalScrollBar()->sliderPosition();
        if (index < ui_item_set.size())
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

    if (inactivate(current_widget))
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
