#pragma once

#include<QtGui>

enum FileSizeType
{
    fileSizeType_Byte,
    fileSizeType_KiloByte,
    FileSizeType_MegaByte,
    fileSizeType_GigaByte
};

struct TreeNode
{
    TreeNode()
    {
        isDir = false;
        isSystemLink = false;
        isFile = false;
    }
    ~TreeNode()
    {
        if(!children.isEmpty())
            qDeleteAll(children);
    }
    QString fileName;
    QIcon fileIcon;
    qint64 fileSize;
    QString fileType;
    QString modifyDate;
    QString dirPath;
    QString filePath;
    bool isDir;
    bool isSystemLink;
    bool isFile;
    QList<TreeNode*>children;

};

QString fileSizeUintTranslator(qint64 size);
bool delDir(const QString &filePath);
