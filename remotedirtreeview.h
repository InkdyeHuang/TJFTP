#ifndef REMOTEDIRTREEVIEW_H
#define REMOTEDIRTREEVIEW_H
#include "widget_item.h"
class RemoteDirWidget;
class  RemoteDirTreeView: public QTreeView
{
    Q_OBJECT
public:
    RemoteDirTreeView(QWidget * parent);
    ~RemoteDirTreeView();
    void resizeColumnsToContents();
private:
    RemoteDirWidget * parentRemoteDirWidget;
};

#endif // REMOTEDIRTREEVIEW_H
