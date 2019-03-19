#include "dirtreemodel.h"

DirTreeModel::DirTreeModel(QObject *parent):QAbstractItemModel(parent)
{
    rootNode = 0;
    if(parent)
    {

    }
}

DirTreeModel::~DirTreeModel()
{

}

int DirTreeModel::rowCount(const QModelIndex &parent) const
{
    if(!rootNode || parent.column() > 0)
        return 0;
    return rootNode->children.count();
}
int DirTreeModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant DirTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !rootNode || rootNode->children.isEmpty() || index.row() > rootNode->children.count())
    {
        return QVariant();
    }
    TreeNode * fileNode = reinterpret_cast<TreeNode *>(index.internalPointer());
    if(index.column() == 0)
    {
        if(role == Qt::DecorationRole)
            return fileNode->fileName;
        else if(role == Qt::DisplayRole || role == Qt::EditRole)
            return fileNode->fileName;
    }
    else if(index.column() == 1)
    {
        if(role == Qt::DisplayRole && !fileNode->isDir && !fileNode->isSystemLink)
            return fileSizeUintTranslator(fileNode->fileSize);
    }
    else if(index.column() == 2)
    {
        if(role == Qt::DisplayRole)
            return fileNode->fileType;
        else if(role == Qt::TextAlignmentRole)
        {
            return int(Qt::AlignLeft | Qt::AlignCenter);
        }
    }
    else if(index.column() == 3)
    {
        if(role == Qt::DisplayRole)
            return fileNode->modifyDate;
        else if(role == Qt::TextAlignmentRole)
            return int(Qt::AlignLeft | Qt::AlignCenter);
    }
    return  QVariant();
}

bool DirTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || !rootNode || rootNode->children.isEmpty() || index.row() > rootNode->children.count())
    {
        return false;
    }
    TreeNode * fileNode = reinterpret_cast<TreeNode *>(index.internalPointer());
    if(index.column() == 0 && role == Qt::EditRole){
        fileNode->fileName = value.toString();
        emit editingFinished(index);
    }
    return true;
}

QVariant DirTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        QString title;
        switch (section) {
        case 0:
            title = tr("标题");
            break;
        case 1:
            title = tr("大小");
            break;
        case 2:
            title = tr("类型");
            break;
        case 3:
            title = tr("修改时间");
        }
        return  title;
    }
    return QVariant();
}

void DirTreeModel::sort(int column, Qt::SortOrder order)
{
    if(!rootNode)
        return;
    if(rootNode->children.count() > 1)
    {
        QList<TreeNode *>::Iterator beg = rootNode->children.begin();
        if((*beg)->fileName == tr(".."))
            beg = beg + 1;
        qStableSort(beg,rootNode->children.end(),)
    }
}

void DirTreeModel::setRootPath(const QString path)
{
    QDir rootPath(QDir::cleanPath(path));
    if(!rootPath.exists())
        return;

}
