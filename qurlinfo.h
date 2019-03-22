#ifndef QURLINFO_H
#define QURLINFO_H

#include<QtCore/qdatetime.h>
#include<QtCore/qstring.h>
#include<QtCore/qiodevice.h>

class QUrl;
class QUrlInforPrivate;

class QUrlInfo
{
public:
    enum PermissionSpec {
        ReadOwner = 00400, WriteOwner = 00200, ExeOwner = 00100,
        ReadGroup = 00040, WriteGroup = 00020, ExeGroup = 00010,
        ReadOther = 00004, WriteOther = 00002, ExeOther = 00001 };
    QUrlInfo();
    QUrlInfo(const QUrlInfo &urlInfo);
    QUrlInfo(const QString &name, int permissions, const QString &owner,
             const QString & group,qint64 size,const QDateTime &lastModified,
             const QDateTime &lastRead, bool isDir, bool isFile, bool isSymLink,
             bool isWritable, bool isReadable, bool isExecutable);
    QUrlInfo(const QUrl &url, int permissions, const QString &owner,
             const QString & group,qint64 size,const QDateTime &lastModified,
             const QDateTime &lastRead, bool isDir, bool isFile, bool isSymLink,
             bool isWritable, bool isReadable, bool isExecutable);
    QUrlInfo &operator=(const QUrlInfo &urlInfo);

    static bool greaterThan(const QUrlInfo &urlInfo1, const QUrlInfo &urlInfo2, int sortBy);
    static bool lessThan(const QUrlInfo &urlInfo1, const QUrlInfo &urlInfo2, int sortBy);
    static bool equal(const QUrlInfo &urlInfo1, const QUrlInfo &urlInfo2, int sortBy);
    bool operator==(const QUrlInfo & urlInfo) const;
    bool operator !=(const QUrlInfo &urlInfo) const
    {
        return !operator==(urlInfo);
    }

    virtual ~QUrlInfo();
    virtual void setName(const QString &name);
    virtual void setPermissions(int permissions);
    virtual void setOwner(const QString &owner);
    virtual void setGroup(const QString &group);
    virtual void setSize(qint64 size);
    virtual void setLastRead(const QDateTime &lastRead);
    virtual void setLastModified(const QDateTime &lastModified);
    virtual void setDir(bool isDir);
    virtual void setFile(bool isFile);
    virtual void setSymLink(bool isSymLink);
    virtual void setWritable(bool isWirtable);
    virtual void setReadable(bool isReadable);
    virtual void setExecutable(bool isExecutable);

    bool isValid() const;

    QString name() const;
    int permissions() const;
    QString owner() const;
    QString group() const;
    qint64 size() const;
    QDateTime lastRead() const;
    QDateTime lastModified() const;
    bool isDir() const;
    bool isFile() const;
    bool isSymLink() const;
    bool isWritable() const;
    bool isReadable() const;
    bool isExecutable() const;

private:
    QUrlInforPrivate *d;
};

#endif // QURLINFO_H
