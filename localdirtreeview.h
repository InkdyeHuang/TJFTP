#ifndef LOCALDIRTREEVIEW_H
#define LOCALDIRTREEVIEW_H
#include "widget_item.h"
#include "localdirwidget.h"
class LocalDirWidget;
class LocalDirTreeView:public QTreeView
{
    Q_OBJECT
public:
    LocalDirTreeView(QWidget *parent);
    ~LocalDirTreeView();
    void resizeColumnsToContents();
private:
    LocalDirWidget * parentLocalDirWidget;
};

#endif // LOCALDIRTREEVIEW_H
