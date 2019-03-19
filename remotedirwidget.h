#ifndef REMOTEDIRTABWIDGET_H
#define REMOTEDIRTABWIDGET_H
#include "widget_item.h"

class TJFTP;
class RemoteDirTreeView;
class RemoteDirWidget : public QWidget
{
    Q_OBJECT
public:
    RemoteDirWidget(QWidget * parent);
    ~RemoteDirWidget();
private:
    TJFTP * parentTJFTP;
    RemoteDirTreeView *remoteDirTreeView;

};
#endif // REMOTEDIRTABWIDGET_H
