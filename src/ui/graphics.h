#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QWidget>
#include <QGroupBox>
#include <QScrollArea>
#include <QFileDialog>
#include <QBoxLayout>
#include <QGridLayout>

#include <QListWidget>
#include <QListWidgetItem>
#include <QTreeWidget>

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QScrollBar>
#include <QSpacerItem>
#include <QProgressBar>

#include "core/core.h"
#include "core/algmanager.h"
#include "core/calculator.h"

namespace Ui
{
    class Graphics;
}

class Graphics : public QWidget
{
    Q_OBJECT
public:
    explicit Graphics(QWidget *parent = nullptr);
    ~Graphics();

    int run();

private:
    void setupPatterns();

    void setupTabCalc();
    void setupTabTable();
    void setupTabTool();
    void setupTabConf();

    bool activate(int index);
    bool inactivate(int index);
    void switchTab(int n);

    void updateCurrentGroup(QString name);
    void startCalculate();

    int current_widget;
    int stored_pos[4];

    Ui::Graphics *ui;
    QList<QList<QWidget *>> ui_item_set;

    QList<DataTableInfo> table_info_list;
    QList<AlgorithmInfo> algorithm_list;
    Group all_enchantments;

    MCE selected_edition;
    DataTableInfo selected_table;
    Group current_group;
    ICM selected_itemconfig;
    EnchList original_enchantments;
    EnchList required_enchantments;
    ItemPool item_pool;
    QString selected_algorithm;
    ALGCFG selected_algconfig;
    FlowStack output_flows;

    Calculator calculator;

signals:
    void currentTabChanged();

    void tableListChanged();
    void tableContentChanged(int n);
    void settingsChanged();

    void currentEditionChanged(MCE mce);
    void currentTableChanged(int n);
    void currentGroupChanged(QString group_name);
    void currentItemConfgChanged(ICM icm);
    void originalEnchListChanged(const EnchList &enchs);
    void requriedEnchListChanged(const EnchList &enchs);
    void itemPoolChanged(const ItemPool &ip);
    void currentAlgorithmChanged(QString alg_name);
    void algorithmConfigChanged(ALGCFG algcfg);

    void calculateStarted();
    void calculatePaused();
    void calculateStopped();
    void calculateFinished();

    void currentFlowChanged(int index);
};

#endif // GRAPHICS_H
