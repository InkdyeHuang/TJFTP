#include "remotedirtreeview.h"
#include "remotedirwidget.h"
#include "tjftp.h"

RemoteDirTreeView::RemoteDirTreeView(QWidget *parent):QTreeView (parent)
{
    parentRemoteDirWidget = reinterpret_cast<RemoteDirWidget *>(parent);
}

RemoteDirTreeView::~RemoteDirTreeView()
{

}
void RemoteDirTreeView::resizeColumnsToContents()
{
    for(int i = 0; i < model()->columnCount(); i++)
        resizeColumnToContents(i);
}

