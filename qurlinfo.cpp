#include "qurlinfo.h"
#include "qurl.h"
#include "qdir.h"

class QUrlInforPrivate
{
public:
    QUrlInforPrivate():
        permissions(0),
        size(0),
        isDir(false),
        isFile(false),
        isSymLink(false),
        isWritable(true),
        isReadable(true),
        isExecutable(false)
    {}

    QString name;
    int permissions;
    QString owner;
    QString group;
    qint64 size;

    QDateTime lastModified;
    QDateTime lastRead;
    bool isDir;
    bool isFile;
    bool isSymLink;
    bool isWritable;
    bool isReadable;
    bool isExecutable;
};

QUrlInfo::QUrlInfo()
{
    d = 0;
}

QUrlInfo::QUrlInfo(const QUrlInfo & urlInfo)
{
    if(urlInfo.d)
    {
        d = new QUrlInforPrivate;
        *d = *urlInfo.d;
    }
    else {
        d = 0;
    }
}

QUrlInfo::QUrlInfo(const QString &name, int permissions, const QString &owner,
         const QString & group,qint64 size,const QDateTime &lastModified,
         const QDateTime &lastRead, bool isDir, bool isFile, bool isSymLink,
         bool isWritable, bool isReadable, bool isExecutable)
{
    d = new QUrlInforPrivate;
    d->name = name;
    d->permissions = permissions;
    d->owner = owner;
    d->group = group;
    d->size = size;
    d->lastRead = lastRead;
    d->lastModified = lastModified;
    d->isDir = isDir;
    d->isFile = isFile;
    d->isSymLink = isSymLink;
    d->isWritable = isWritable;
    d->isReadable = isReadable;
    d->isExecutable = isExecutable;
}

QUrlInfo::QUrlInfo(const QUrl &url, int permissions, const QString &owner,
         const QString & group,qint64 size,const QDateTime &lastModified,
         const QDateTime &lastRead, bool isDir, bool isFile, bool isSymLink,
         bool isWritable, bool isReadable, bool isExecutable)
{
    d = new QUrlInforPrivate;
    d->name = QFileInfo(url.path()).fileName();
    d->permissions = permissions;
    d->owner = owner;
    d->group = group;
    d->size = size;
    d->lastRead = lastRead;
    d->lastModified = lastModified;
    d->isDir = isDir;
    d->isFile = isFile;
    d->isSymLink = isSymLink;
    d->isWritable = isWritable;
    d->isReadable = isReadable;
    d->isExecutable = isExecutable;
}

//等号赋值？这样写感觉还要额外调用一次拷贝构造函数
QUrlInfo &QUrlInfo::operator=(const QUrlInfo &urlInfo)
{
    if(urlInfo.d)
    {
        if(!d)
            d = new QUrlInforPrivate;
        *d = *urlInfo.d;
    }
    else {
        delete d;
        d = 0;
    }
    return *this;

}

QUrlInfo::~QUrlInfo()
{
    if(d)
    {
        delete d;
        d= 0;
    }
}

bool QUrlInfo::greaterThan(const QUrlInfo &urlInfo1, const QUrlInfo &urlInfo2, int sortBy)
{
    switch (sortBy) {
    case QDir::Name:
        return urlInfo1.name() > urlInfo2.name();
    case QDir::Time:
        return urlInfo1.lastModified() > urlInfo2.lastModified();
    case QDir::Size:
        return urlInfo1.size() > urlInfo2.size();
    default:
        return false;
    }
}

bool QUrlInfo::lessThan(const QUrlInfo &urlInfo1, const QUrlInfo &urlInfo2, int sortBy)
{
    return !greaterThan(urlInfo1,urlInfo2,sortBy);
}

bool QUrlInfo::equal(const QUrlInfo &urlInfo1, const QUrlInfo &urlInfo2, int sortBy)
{
    switch (sortBy) {
    case QDir::Name:
        return urlInfo1.name() == urlInfo2.name();
    case QDir::Time:
        return urlInfo1.lastModified() == urlInfo2.lastModified();
    case QDir::Size:
        return urlInfo1.size() == urlInfo2.size();
    default:
        return false;
    }
}

bool QUrlInfo::operator==(const QUrlInfo &urlInfo) const
{
    if(!d)
        return urlInfo.d == 0;
    if(!urlInfo.d)
        return false;
    return (d->name == urlInfo.d->name &&
            d->permissions == urlInfo.d->permissions &&
            d->owner == urlInfo.d->owner &&
            d->group == urlInfo.d->group &&
            d->size == urlInfo.d->size &&
            d->lastModified == urlInfo.d->lastModified &&
            d->lastRead == urlInfo.d->lastRead &&
            d->isDir == urlInfo.d->isDir &&
            d->isFile == urlInfo.d->isFile &&
            d->isSymLink == urlInfo.d->isSymLink &&
            d->isReadable == urlInfo.d->isReadable &&
            d->isWritable == urlInfo.d->isWritable &&
            d->isExecutable == urlInfo.d->isExecutable);
}

void QUrlInfo::setName(const QString &name)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->name = name;
}

void QUrlInfo::setPermissions(int permissions)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->permissions = permissions;
}

void QUrlInfo::setOwner(const QString &owner)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->owner = owner;
}

void QUrlInfo::setGroup(const QString &group)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->group = group;
}

void QUrlInfo::setSize(qint64 size)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->size = size;
}

void QUrlInfo::setLastModified(const QDateTime &lastModified)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->lastModified = lastModified;
}

void QUrlInfo::setLastRead(const QDateTime &lastRead)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->lastRead = lastRead;
}

void QUrlInfo::setDir(bool isDir)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->isDir = isDir;
}

void QUrlInfo::setFile(bool isFile)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->isFile = isFile;
}

void QUrlInfo::setSymLink(bool isSymLink)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->isSymLink = isSymLink;
}

void QUrlInfo::setReadable(bool isReadable)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->isReadable = isReadable;
}

void QUrlInfo::setWritable(bool isWirtable)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->isWritable = isWirtable;
}

void QUrlInfo::setExecutable(bool isExecutable)
{
    if(!d)
        d = new QUrlInforPrivate;
    d->isExecutable = isExecutable;
}

QString QUrlInfo::name() const
{
    if(!d)
        return QString();
    return d->name;
}

int QUrlInfo::permissions() const
{
    if(!d)
        return 0;
    return d->permissions;
}

QString QUrlInfo::owner() const
{
    if(!d)
        return QString();
    return d->owner;
}

QString QUrlInfo::group() const
{
    if(!d)
        return QString();
    return d->group;
}

qint64 QUrlInfo::size() const
{
    if(!d)
        return 0;
    return d->size;
}

QDateTime QUrlInfo::lastModified() const
{
    if(!d)
        return QDateTime();
    return  d->lastModified;
}

QDateTime QUrlInfo::lastRead() const
{
    if(!d)
        return QDateTime();
    return d->lastRead;
}

bool QUrlInfo::isDir() const
{
    if(!d)
        return false;
    return d->isDir;
}

bool QUrlInfo::isFile() const
{
    if(!d)
        return false;
    return d->isFile;
}

bool QUrlInfo::isSymLink() const
{
    if(!d)
        return false;
    return d->isFile;
}

bool QUrlInfo::isWritable() const
{
    if(!d)
        return false;
    return d->isWritable;
}

bool QUrlInfo::isReadable() const
{
    if(!d)
        return false;
    return d->isReadable;
}

bool QUrlInfo::isExecutable() const
{
    if(!d)
        return false;
    return d->isExecutable;
}

bool QUrlInfo::isValid() const
{
    return d!=0;
}
