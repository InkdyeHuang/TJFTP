#ifndef LOCALDIRTABWIDGET_H
#define LOCALDIRTABWIDGET_H
#include "widget_item.h"
class TJFTP;
class LocalDirTreeView;
class LocalDirWidget: public QWidget
{
    Q_OBJECT
public:
    LocalDirWidget(QWidget *parent);
    ~LocalDirWidget();
private:
    TJFTP *parentTJFTP;
    LocalDirTreeView *localDirTreeView;
};

#endif // LOCALDIRTABWIDGET_H
