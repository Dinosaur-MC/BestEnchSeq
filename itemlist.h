#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QListWidget>
#include "datamanager.h"
#include "itemlistwidget.h"

class ItemList : public QListWidget
{
    Q_OBJECT
public:
    explicit ItemList(QWidget *parent = nullptr);

    void reinit();  //初始化
    int itemCount();    //被选中物品的数量
    void refresh(); //刷新
    void uploadList();  //上传数据至数据管理器

    void itemClickedOperation();    //单击切换选中状态或添加物品
    void itemDoubleClickedOperation();  //双击添加或修改物品

private:
    Item this_item;
    ItemPool *item_pool;

signals:
    void resized();

protected:
//     void contextMenuEvent(QContextMenuEvent *event); //删除了删除功能（

};

#endif // ITEMLIST_H
