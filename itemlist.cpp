#include "itemlist.h"
#include "itemeditor.h"

ItemList::ItemList(QWidget *parent)
    : QListWidget{parent}
{
    qDebug() << "[ItemList]";
    this->setViewMode(QListView::ListMode);
    this->setResizeMode(QListView::Adjust);

    connect(this, &ItemList::itemClicked, this, &ItemList::itemClickedOperation);
    connect(this, &ItemList::itemDoubleClicked, this, &ItemList::itemDoubleClickedOperation);

    this_item = {};
    item_pool = new ItemPool();
    reinit();
}


void ItemList::reinit() //初始化
{
    clear();
    item_pool->clear();

    QListWidgetItem *item = new QListWidgetItem("(新建物品 New Item)");
    item->setSizeHint(QSize(200,40));
    addItem(item);  //添加常驻末项
}

int ItemList::itemCount()   //被选中物品的数量
{
    int n = 0;
    for(int i = 0; i < count(); i++)
    {
        ItemListWidget *w = (ItemListWidget*)itemWidget(item(i));
        if(w->isChecked())
            n++;
    }
    return n;
}

void ItemList::uploadList() //上传数据至数据管理器
{
//    Ench *e = new Ench[count()];

//    int n = 0;
//    for(int i = 0; i < count(); i++)
//    {
//        ItemListWidget *w = (ItemListWidget*)itemWidget(item(i));
//        if(w->isChecked())
//        {
//            e[n] = w->ench();
//            n++;
//        }
//    }

//    item_pool->clear();
//    for(int i = 0; i < n; i++)
//        item_pool->append(ID_ECB, e, n, 0, 0);
//    delete [] e;

    DM->upload(item_pool);

    qDebug() << "uploadList - item_pool: Done";
}

void ItemList::itemClickedOperation()   //单击切换选中状态或添加物品
{
    if(currentRow()+1 < count())
    {
        ItemListWidget *w = (ItemListWidget*)itemWidget(currentItem());
        w->setChecked(!w->isChecked());
    }
    else
    {
        itemDoubleClickedOperation();
    }
}

void ItemList::itemDoubleClickedOperation() //双击添加或修改物品
{
    ItemEditor *w = new ItemEditor();
    ItemListWidget *iw = nullptr;

    if(currentRow()+1 < count())
    {
        iw = (ItemListWidget*)itemWidget(currentItem());
        w->setItem(iw->item());
    }

    w->setModal(true);
    w->show();
    if(w->exec() == ItemEditor::Accepted)
        this_item = w->item();
    else
        return;

    iw = nullptr;
    iw = new ItemListWidget(this);
    iw->setItem(this_item);
    if(currentRow()+1 == count())
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(iw->sizeHint().width(), iw->sizeHint().height()));
        insertItem(currentRow(), item);
        setItemWidget(item, iw);
    }
    else
    {
        removeItemWidget(currentItem());
        currentItem()->setSizeHint(QSize(iw->sizeHint().width(), iw->sizeHint().height()));
        setItemWidget(currentItem(), iw);
    }
    item_pool->append(this_item);
}

