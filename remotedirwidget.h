#ifndef REMOTEDIRTABWIDGET_H
#define REMOTEDIRTABWIDGET_H

//#include <QFtp>
#include "widget_item.h"

class TJFTP;
class RemoteDirTreeView;
class DirTreeModel;
class RemoteDirWidget : public QWidget
{
    Q_OBJECT
public:
    friend RemoteDirTreeView;
    RemoteDirWidget(QWidget * parent);
    ~RemoteDirWidget();
    void connectToHost(const QString &address,const QString &port,const QString &username = QString(),const QString &pwd = QString());
    bool isConnected() const;
    void reset();
    void reconnect();
    void disconnect();
private:
    TJFTP * parentTJFTP;
    RemoteDirTreeView *remoteDirTreeView;
    DirTreeModel *remoteDirTreeModel;
    QFileSystemModel *remoteDirFileSystemModel;
    QTreeView remoteDIrComboTreeView;
    QComboBox *remoteDirComboBox;
    QToolButton *connectToolButton;
    QToolButton *dotdotToolButton;
    QToolButton *refreshToolButton;
    QStatusBar *remoteDirStatusBar;
    QTextEdit *logTextEdit;


};
#endif // REMOTEDIRTABWIDGET_H
