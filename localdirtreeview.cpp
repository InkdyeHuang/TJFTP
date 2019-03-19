#include "localdirtreeview.h"

LocalDirTreeView::LocalDirTreeView(QWidget * parent):QTreeView (parent)
{
    parentLocalDirWidget = reinterpret_cast<LocalDirWidget *>(parent);
}
LocalDirTreeView:: ~LocalDirTreeView()
{

}
void LocalDirTreeView::resizeColumnsToContents()
{
    for(int i = 0; i < model()->columnCount();i++)
        resizeColumnToContents(i);
}

