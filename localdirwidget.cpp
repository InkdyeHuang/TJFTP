#include "localdirwidget.h"
#include "localdirtreeview.h"
#include "tjftp.h"

LocalDirWidget::LocalDirWidget(QWidget * parent):QWidget (parent)
{
    parentTJFTP = reinterpret_cast<TJFTP *>(parent);
    localDirTreeView = new LocalDirTreeView(this);
}
LocalDirWidget:: ~LocalDirWidget()
{

}
