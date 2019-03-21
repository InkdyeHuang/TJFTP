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
    void currentIndexChanged(const QString &text);
    void showContextMenu(const QModelIndex &index);
    void editingFinished(const QModelIndex &index);
    void dotdot();
    void refresh();
    void del();
    void rename();
    void newDir();
    void property();
    void upload();

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
    QMenu *contextMenu;
    QAction *delAction;
    QAction *renameAction;
    QAction *newDirAction;
    QAction *propertyAction;
    QAction *uploadAction;

};

#endif // LOCALDIRTABWIDGET_H
