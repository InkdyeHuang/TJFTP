#include "remotedirwidget.h"
#include "remotedirtreeview.h"
RemoteDirWidget::RemoteDirWidget(QWidget *parent):QWidget (parent)
{
    parentTJFTP = reinterpret_cast<TJFTP *>(parent);
    remoteDirTreeView = new RemoteDirTreeView(this);
}

RemoteDirWidget::~RemoteDirWidget()
{

}
