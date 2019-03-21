#include "localdirwidget.h"
#include "localdirtreeview.h"
#include "dirtreemodel.h"
#include "tjftp.h"

LocalDirWidget::LocalDirWidget(QWidget * parent):QWidget (parent)
{
    parentTJFTP = reinterpret_cast<TJFTP *>(parent);

    localDirTreeModel = new DirTreeModel(this);
    localDirTreeModel->setRootPath(QDir::currentPath());

    localDirTreeView = new LocalDirTreeView(this);
    localDirTreeView->setModel(localDirTreeModel);
    localDirTreeView->header()->setStretchLastSection(true);
    localDirTreeView->resizeColumnsToContents();
    localDirTreeView->setAlternatingRowColors(true);
    localDirTreeView->setSelectionMode(QAbstractItemView::ContiguousSelection);
    localDirTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    localDirTreeView->setSortingEnabled(true);
    localDirTreeView->sortByColumn(0,Qt::AscendingOrder);
    localDirTreeView->setItemsExpandable(false);
    localDirTreeView->setRootIsDecorated(false);
    localDirTreeView->setExpandsOnDoubleClick(false);
    localDirTreeView->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked);

    localDirSystemModel = new QFileSystemModel(this);
    localDirSystemModel->setFilter(QDir::AllDirs | QDir::Drives | QDir::NoDot | QDir::NoDotAndDotDot);
    QModelIndex index = localDirSystemModel->setRootPath(QDir::currentPath());

    localDirComboTreeView = new QTreeView(this);
    localDirComboTreeView->setModel(localDirSystemModel);
    localDirComboTreeView->resizeColumnToContents(0);
    localDirComboTreeView->hideColumn(1);
    localDirComboTreeView->hideColumn(2);
    localDirComboTreeView->hideColumn(3);
    localDirComboTreeView->setHeaderHidden(true);
    localDirComboTreeView->expand(index);
    localDirComboTreeView->scrollTo(index);
    localDirComboTreeView->setCurrentIndex(index);
    localDirComboTreeView->setItemsExpandable(true);

    localDirComboBox = new QComboBox(this);
    localDirComboBox->setModel(localDirSystemModel);
    localDirComboBox->setView(localDirComboTreeView);

    dotdotDirToolButton = new QToolButton(this);
    dotdotDirToolButton->setText(tr("上级目录"));
    refreshDirToolButton = new QToolButton(this);
    refreshDirToolButton->setText(tr("刷新"));
    localDirStatusBar = new QStatusBar(this);
    QHBoxLayout *topHBoxLayout = new QHBoxLayout;
    topHBoxLayout->addWidget(dotdotDirToolButton);
    topHBoxLayout->addWidget(refreshDirToolButton);
    topHBoxLayout->addWidget(localDirComboBox);
    QVBoxLayout *mainLayeout = new QVBoxLayout;
    mainLayeout->addLayout(topHBoxLayout);
    mainLayeout->addWidget(localDirTreeView);
    mainLayeout->addWidget(localDirStatusBar);

    setLayout(mainLayeout);

    contextMenu = new QMenu(this);
    delAction = new QAction(tr("删除"),this);
    renameAction = new QAction(tr("重命名"),this);
    newDirAction = new QAction(tr("新建文件夹"),this);

    propertyAction = new QAction(tr("属性"),this);

    uploadAction = new QAction(tr("上传"),this);

    contextMenu->addAction(uploadAction);
    contextMenu->addSeparator();
    contextMenu->addAction(delAction);
    contextMenu->addAction(renameAction);
    contextMenu->addAction(newDirAction);
    contextMenu->addSeparator();
    contextMenu->addAction(propertyAction);


    connect(localDirTreeView,SIGNAL(doubleClicked(const QModelIndex &)),this,SLOT(setRootIndex(const QModelIndex &)));
    connect(localDirTreeView,SIGNAL(pressed(const QModelIndex &)),this,SLOT(showContextMenu(const QModelIndex &)));
    connect(dotdotDirToolButton,SIGNAL(clicked()),this,SLOT(dotdot()));
    connect(refreshDirToolButton,SIGNAL(clicked()),this,SLOT(refresh()));
    connect(localDirComboBox,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(currentIndexChanged(const QString &)));
    connect(localDirTreeModel,SIGNAL(editingFinished(const QModelIndex)),this,SLOT(editingFinished(const QModelIndex &)));
    //contextMenu signals and slots
    connect(uploadAction,SIGNAL(triggered()),this,SLOT(upload()));
    connect(delAction,SIGNAL(triggered()),this,SLOT(del()));
    connect(renameAction,SIGNAL(triggered()),this,SLOT(rename()));
    connect(newDirAction,SIGNAL(triggered()),this,SLOT(newDir()));
    connect(propertyAction,SIGNAL(triggered()),this,SLOT(property()));
}
LocalDirWidget:: ~LocalDirWidget()
{
}

QDir LocalDirWidget::currentDir(bool *ok) const
{
    return localDirTreeModel->currentDir(ok);
}

QString LocalDirWidget::currentDirPath() const
{
    return localDirTreeModel->currentDirPath();
}
QString LocalDirWidget::currentFilePath() const
{
    TreeNode *node = reinterpret_cast<TreeNode *>(localDirTreeView->currentIndex().internalPointer());
    if(!node)
        return QString();
    return node->filePath;
}

void LocalDirWidget::setRootIndex(const QModelIndex &index)
{
    if(!index.isValid())
        return ;
    TreeNode *node = reinterpret_cast<TreeNode *>(index.internalPointer());
    if(node->fileName == tr("..")){
        dotdot();
    }
    else if(node->isDir)
    {
        dotdotDirToolButton->setEnabled(true);
        QString dir = node->filePath;
        localDirTreeModel->setRootIndex(index);
        localDirTreeView->resizeColumnsToContents();
    }
}

void LocalDirWidget::dotdot()
{
    localDirTreeModel->setRootPath(currentDirPath() + tr("/") + tr(".."));
    localDirTreeView->reset();
    localDirTreeView->resizeColumnsToContents();
    TreeNode *dotdotNode = reinterpret_cast<TreeNode *>(localDirTreeModel->index(0,0).internalPointer());
    dotdotDirToolButton->setEnabled(dotdotNode->fileName == tr(".."));
}
void LocalDirWidget::refresh()
{
    localDirTreeModel->setRootPath(currentDirPath());
    localDirTreeView->reset();
    localDirTreeView->resizeColumnsToContents();

}
void LocalDirWidget::currentIndexChanged(const QString &text)
{
    QModelIndex curIndex = localDirComboTreeView->currentIndex();
    localDirComboTreeView->collapseAll();
    localDirComboTreeView->expand(curIndex);
    localDirComboTreeView->reset();
    localDirTreeModel->setRootPath(localDirSystemModel->filePath(curIndex));
    localDirTreeView->reset();
    localDirTreeView->resizeColumnsToContents();
}

void LocalDirWidget::showContextMenu(const QModelIndex &index)
{
    if(QApplication::mouseButtons() == Qt::RightButton)
    {
        QList<QAction *> actions = contextMenu->actions();
        foreach(QAction * action, actions)
            action->setEnabled(true);
        TreeNode * node = reinterpret_cast<TreeNode *>(index.internalPointer());
        if(node->isDir && node->fileName == tr(".."))
        {
            foreach(QAction * action,actions)
                action->setEnabled(false);
        }
        contextMenu->exec(QCursor::pos());
    }
}

void LocalDirWidget::editingFinished(const QModelIndex &index)
{
    TreeNode *n = reinterpret_cast<TreeNode *>(index.internalPointer());
    if(QFileInfo(n->filePath).fileName() != n->fileName)
    {
        QString oldfilePath = n->filePath;
        n->filePath = n->dirPath + tr("/") + n->fileName;
        QFile::rename(oldfilePath,n->filePath);
    }
}
void LocalDirWidget::upload()
{

}
void LocalDirWidget::del()
{
    QString filePath = currentFilePath();
    QFileInfo fileInfo(filePath);
    if(fileInfo.isDir())
    {
        if(delDir(filePath))
            refresh();
        else {
            QMessageBox::about(this,"删除失败","文件夹文件可能被占用");
        }
    }
    else {
        if(QFile(filePath).remove())
            refresh();
        else {
            QMessageBox::about(this,"删除失败","文件可能被占用");
        }
    }

}

void LocalDirWidget::rename()
{
    qDebug()<<currentFilePath();
}

void LocalDirWidget::newDir()
{
    QString dirName = tr("新建文件夹");
    QDir dir(currentDirPath());
    int index = 1;
    while(dir.exists(dirName))
        dirName = tr("新建文件夹(%1)").arg(index++);
    dir.mkdir(dirName);
    refresh();
}
void LocalDirWidget::property()
{

}
