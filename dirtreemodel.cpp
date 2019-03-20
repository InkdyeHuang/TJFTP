#include "dirtreemodel.h"

DirTreeModel::DirTreeModel(QObject *parent):QAbstractItemModel(parent)
{
    rootNode = 0;
    if(parent)
    {
        connect(this,SIGNAL(editingFinished(const QModelIndex &)),parent,SLOT(editingFinished(const QModelIndex &)));
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
            return fileNode->fileIcon;
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
            title = tr("名称");
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
        std::stable_sort(beg,rootNode->children.end(),[&](const TreeNode *fileNode1, const TreeNode * fileNode2)->bool{
            if((fileNode1->isDir && fileNode2->isDir)||(!fileNode1->isDir && !fileNode2->isDir))
            {
                if(column == 0)
                {
                    if(order == Qt::AscendingOrder)
                        return fileNode1->fileName.toLower() < fileNode2->fileName.toLower();
                    return  fileNode1->fileName.toLower() > fileNode2->fileName.toLower();
                }
                else if(column == 1)
                {
                    if(order == Qt::AscendingOrder)
                        return  fileNode1->fileSize < fileNode2->fileSize;
                    return  fileNode1->fileSize > fileNode2->fileSize;
                }
                else if(column == 2)
                {
                    if(order == Qt::AscendingOrder)
                        return fileNode1->fileType < fileNode2->fileType;
                    return  fileNode1->fileType > fileNode2->fileType;
                }
                else if(column == 3)
                {
                    if(order == Qt::AscendingOrder)
                        return fileNode1->modifyDate < fileNode2->modifyDate;
                    return  fileNode1 ->modifyDate > fileNode2->modifyDate;
                }
            }
            return false;//?
        });
        //reset();
    }
}

void DirTreeModel::setRootPath(const QString path)
{
    QDir rootPath(QDir::cleanPath(path));
    if(!rootPath.exists())
        return;
    if(rootNode)
        delete rootNode;

    QFileInfo fileInfo(path);
    rootNode = new TreeNode;
    rootNode->fileName = fileInfo.fileName();
    rootNode->fileIcon = provider.icon(fileInfo);
    rootNode->fileSize = fileInfo.size();
    rootNode->fileType = provider.type(fileInfo);
    rootNode->modifyDate = fileInfo.lastModified().toString("yyyy/MM/dd hh:mm");
    rootNode->isDir = fileInfo.isDir();
    rootNode->isSystemLink = fileInfo.isSymLink();
    rootNode->isFile = fileInfo.isFile();
    rootNode->dirPath = QDir::fromNativeSeparators(fileInfo.absolutePath());
    rootNode->filePath = QDir::fromNativeSeparators(fileInfo.absoluteFilePath());

    foreach(QFileInfo fileInfo, rootPath.entryInfoList(QDir::NoDot | QDir::AllEntries, QDir::DirsFirst | QDir::IgnoreCase | QDir::Name)){
        TreeNode *p = new TreeNode;
        p->fileName = fileInfo.fileName();
        p->fileIcon = provider.icon(fileInfo);
        p->fileSize = fileInfo.size();
        p->fileType = provider.type(fileInfo);
        p->modifyDate = fileInfo.lastModified().toString("yyyy/MM/dd hh:mm");
        p->isDir = fileInfo.isDir();
        p->isSystemLink = fileInfo.isSymLink();
        p->isFile = fileInfo.isFile();
        p->dirPath = QDir::fromNativeSeparators(fileInfo.absolutePath());
        p->filePath = QDir::fromNativeSeparators(fileInfo.absoluteFilePath());
        rootNode->children.push_back(p);
    }
    sort(0,Qt::AscendingOrder);
}

void DirTreeModel::setRootIndex(const QModelIndex &index)
{
    if(!index.isValid() || !rootNode || rootNode->children.isEmpty())
        return;
    TreeNode *fileNode = reinterpret_cast<TreeNode *>(index.internalPointer());
    if(fileNode->isDir)
        setRootPath(fileNode->filePath);
}

QModelIndex DirTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!rootNode || rootNode->children.empty() || row >= rootNode->children.count() || row < 0 || parent.column() > 0)
        return QModelIndex();
    TreeNode *childNode = NULL;
    if(!parent.isValid())
        childNode = rootNode->children.at(row);
    else {
        childNode = reinterpret_cast<TreeNode *>(parent.internalPointer())->children.at(row);
    }
    if(childNode)
        return createIndex(row,column,childNode);
    else {
        return  QModelIndex();
    }
}

QModelIndex DirTreeModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

Qt::ItemFlags DirTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;
    TreeNode *n = reinterpret_cast<TreeNode *>(index.internalPointer());
    if(index.column() == 0 && n->fileName != tr(".."))
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    return  QAbstractItemModel::flags(index);
}

QDir DirTreeModel::currentDir(bool *ok) const
{
    if(!rootNode)
    {
        if(ok)
            *ok = false;
        return  QDir();
    }
    else {
        if(ok)
            *ok = true;
        return (rootNode->isDir ? rootNode->filePath : rootNode->dirPath);
    }
}

QString DirTreeModel::currentDirPath() const
{
    if(!rootNode)
        return QString();
    return (rootNode->isDir ? rootNode->filePath : rootNode->dirPath);
}
