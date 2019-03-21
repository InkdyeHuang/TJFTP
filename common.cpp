#include "common.h"
QString fileSizeUintTranslator(qint64 size)
{
    qreal fileSize = size;
    int level = fileSizeType_Byte;

    QString sizeInfo = "";
    while(qFloor(fileSize / 1024))
    {
        fileSize /= 1024.0;
        level++;
        if(level >= fileSizeType_GigaByte)
            break;
    }
    fileSize = QString::number(fileSize,'f',2).toDouble();
    if(level == fileSizeType_Byte)
        sizeInfo = QObject::tr("%1 B").arg(fileSize);
    else if(level == fileSizeType_KiloByte)
        sizeInfo = QObject::tr("%1 KB").arg(fileSize);
    else if(level == FileSizeType_MegaByte)
        sizeInfo = QObject::tr("%1 MB").arg(fileSize);
    else if(level >= fileSizeType_GigaByte)
        sizeInfo = QObject::tr("%1 GB").arg(fileSize);
    return sizeInfo;
}
bool delDir(const QString &filePath)
{
    if(filePath.isEmpty())
        return false;
    QDir dir(filePath);
    if(!dir.exists())
        return true;
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach(QFileInfo file,fileList)
        if(file.isFile())
            file.dir().remove(file.fileName());
        else {
            delDir(file.absoluteFilePath());
        }
    return dir.rmpath(dir.absolutePath());
}
