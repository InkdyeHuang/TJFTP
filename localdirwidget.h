#ifndef LOCALDIRTABWIDGET_H
#define LOCALDIRTABWIDGET_H
#include "widget_item.h"
class TJFTP;
class TabWidget;
class LocalDirTreeView;
class DirTreeModel;
class LocalDirWidget: public QWidget
{
    Q_OBJECT
public:
    friend LocalDirTreeView;
    LocalDirWidget(QWidget *parent);
    ~LocalDirWidget();
    QDir currentDir(bool *ok = 0) const;
    QString currentDirPath() const;
    QString currentFilePath() const;
    void reset();
private slots:
    void setRootIndex(const QModelIndex &index);
    void dotdot();
    void refresh();
    void currentIndexChanged(const QString &text);
private:
    TJFTP *parentTJFTP;
    LocalDirTreeView *localDirTreeView;
    DirTreeModel *localDirTreeModel;
    QFileSystemModel *localDirSystemModel;
    QTreeView *localDirComboTreeView;
    QComboBox *localDirComboBox;
    QToolButton *dotdotDirToolButton;
    QToolButton *refreshDirToolButton;
    QStatusBar *localDirStatusBar;
};

#endif // LOCALDIRTABWIDGET_H
