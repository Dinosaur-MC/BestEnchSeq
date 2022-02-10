#ifndef FLOWLISTWIDGET_H
#define FLOWLISTWIDGET_H

#include <QListWidget>
#include "stepwidget.h"
#include "common.h"


class FlowListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit FlowListWidget(QWidget *parent = nullptr);

    void addStep(Step s, QIcon icon);
    void refresh();
    void refreshSize();

private:
    QListWidget* m_Local = nullptr;

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void Refresh();
    void SizeChanged();

};

#endif // FLOWLISTWIDGET_H
