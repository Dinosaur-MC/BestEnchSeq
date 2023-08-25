#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QWidget>
#include "core/core.h"
#include "core/algmanager.h"

namespace Ui {
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

    void updateOELW(Group group);
    void updateRELW(Group group, EnchList enchs);

    bool activate(int index);
    bool inactivate(int index);
    void switchTab(int n);

    int current_widget;
    int stored_pos[4];

    QList<QList<QWidget *>> ui_item_set;
    Ui::Graphics *ui;

    QList<DataTableInfo> table_info_list;
    QList<AlgorithmInfo> algorithm_list;

    MCE selected_edition;
    DataTableInfo selected_table;
    QString selected_group_name;
    ICM selected_itemconfig;
    QList<Ench> required_enchantments;
    QList<Item> item_pool;
    QString selected_algorithm;
    ALGCFG selected_algconfig;
    FlowStack output_flows;

signals:
    void currentTabChanged();

};

#endif // GRAPHICS_H
