#include "qftp.h"
#include "qabstractsocket.h"
#include "qtcpsocket.h"
#include "qtcpserver.h"
#include "qurlinfo.h"
#include "qlocale.h"
#include "qfileinfo.h"
#include "qhash.h"
#include "qtimer.h"
#include "qregexp.h"
#include "qstringlist.h"
#include "qcoreapplication.h"

#define QFTPDTP_DEBUG

class QFtpPI;
/*
      DTP,Data Transfer Process 数据传输处理类，控制多个客户端和服务器的数据传输
*/
class QFtpDTP:public QObject
{
    Q_OBJECT
public:
    enum ConnectState{
        CsHostFound,
        CsConnected,
        CsClosed,
        CsHostNotFound,
        CsConnectRefused
    };
    QFtpDTP(QFtpPI *p,QObject *parent = 0);
    ~QFtpDTP();
    void setData(QByteArray *);
    void setDevice(QIODevice *);
    void writeData();
    void setBytesTotal(qint64 bytes);

    bool hasError() const;
    QString errorMessage() const;
    void cleanError();

    void connectToHost(const QString & host, quint16 port);
    int setupListener(const QHostAddress &address);
    void waitForConnection();
    void abortConnection();

    QTcpSocket::SocketState state() const;
    qint64 bytesAvailable() const;
    qint64 read(char *data, qint64 maxlen);
    QByteArray readAll();



    static bool parseDir(const QByteArray &buffer, const QString &userName,QUrlInfo *info);
signals:
    void listInfo(const QUrlInfo&);
    void readyRead();
    void dataTransferProgress(qint64,qint64);
    void connectState(int);
private slots:
    void socketConnected();
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError);
    void socketConnectionClosed();
    void socketBytesWritten(qint64);
    void setupSocket();

private:
    void clearData();
    QTcpSocket *socket;
    QTcpServer listener;

    QFtpPI *pi;
    QString err;
    qint64 bytesDone;
    qint64 bytesTotal;
    bool callWriteData;
    //is_ba 为ture的时候用ba，反之用dev;值一样
    union
    {
        QByteArray *ba;
        QIODevice *dev;
    }data;
    bool is_ba;
    QByteArray bytesFromSocket;
};
/*
    PI,Protocol Interpreter, 解析协议类
*/
class QFtpPI:public QObject
{
    Q_OBJECT
public:
    QFtpPI(QObject *parent = 0);
    ~QFtpPI();
    QFtpDTP dtp;//含有一个DTP，不是按照RFC 959设计，但是这样更简单
private:
    friend class QFtpDTP;
};
/*
    命令实现
 */
class QFtpCommand
{
public:
    QFtpCommand(QFtp::Command cmd,QStringList raw, const QByteArray &ba);
    QFtpCommand(QFtp::Command cmd, QStringList raw, QIODevice *dev = 0);
    ~QFtpCommand();
};
//listener
QFtpDTP::QFtpDTP(QFtpPI *p,QObject *parent):
    QObject(parent),
    socket(0),
    listener(this),
    pi(p),
    callWriteData(false)
{
    clearData();
    listener.setObjectName(QLatin1String("QFtpDTP active state server"));
    connect(&listener,SIGNAL(newConnection()),this,SLOT(setupSocket()));

}

void QFtpDTP::setData(QByteArray *ba)
{
    is_ba = true;
    data.ba = ba;
}

void QFtpDTP::setDevice(QIODevice *dev)
{
    is_ba = false;
    data.dev = dev;
}

void QFtpDTP::setBytesTotal(qint64 bytes)
{
      bytesTotal = bytes;
      bytesDone = 0;
      emit dataTransferProgress(bytesTotal,bytesDone);
}

void QFtpDTP::connectToHost(const QString &host, quint16 port)
{
    bytesFromSocket.clear();
    if(socket)
    {
        delete socket;
        socket = 0;
    }
    socket = new QTcpSocket(this);
#ifndef QT_NO_BEARERMANAGEMENT
    socket->setProperty("_q_networksession",property("_q_networksession"));
#endif
    socket->setObjectName(QLatin1String("QFtpDTP Passive state socket"));
    connect(socket,SIGNAL(connected()),this,SLOT(socketConnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(socketReadyRead()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(disconnected()),this,SLOT(socketConnectionClosed()));
    connect(socket,SIGNAL(bytesWritten()),this,SLOT(socketBytesWritten()));
    socket->connectToHost(host,port);
}

int QFtpDTP::setupListener(const QHostAddress &address)
{
#ifndef QT_NO_BEARERMANAGEMENT
    socket->setProperty("_q_networksession",property("_q_networksession"));
#endif
    if(!listener.isListening() && !listener.listen(address,0))
        return -1;
    return listener.serverPort();
}

void QFtpDTP::waitForConnection()
{
    if(listener.isListening())
        listener.waitForNewConnection();
}

void QFtpDTP::abortConnection()
{
#if defined (QFTPDTP_DEBUG)
    qDebug("QFtpDTP::abortConnection, bytesAvailable == %lli",socket ? socket->bytesAvailable() : (qint64) 0);
#endif
    callWriteData = false;
    clearData();

    if(socket)
    socket->abort();

}

QTcpSocket::SocketState QFtpDTP::state() const
{
    return socket ?socket->state() : QTcpSocket::UnconnectedState;
}

qint64 QFtpDTP::bytesAvailable() const
{
    if(!socket || socket->state() != QTcpSocket::ConnectedState)
        return bytesFromSocket.size();
    return socket->bytesAvailable();
}

qint64 QFtpDTP::read(char *data, qint64 maxlen)
{
    qint64 read_num;
    if(socket && socket->state() == QTcpSocket::ConnectingState)
        read_num = socket->read(data,maxlen);
    else {
        read_num = qMin(maxlen,qint64(bytesFromSocket.size()));
        memcpy(data,bytesFromSocket.data(),read_num);
        bytesFromSocket.remove(0,read_num);
    }
    bytesDone += read_num;
    return  read_num;
}

QByteArray QFtpDTP::readAll()
{
    QByteArray tmpByteArray;
    if(socket && socket->state() == QTcpSocket::ConnectedState)
    {
        tmpByteArray = socket->readAll();
    }
    else {
        tmpByteArray = bytesFromSocket;
        bytesFromSocket.clear();
    }
    bytesDone += tmpByteArray.size();
    return tmpByteArray;
}

void QFtpDTP::writeData()
{
    if(!socket)
        return;
    if(is_ba)
    {
#if defined(QFTPDTP_DEBUG)
        qDebug("QFtpDTP::writeData: write %d bytes", data.ba->size());
#endif
        if(data.ba->size() == 0)
            emit dataTransferProgress(0,bytesTotal);
        else {
            socket->write(data.ba->data(),data.ba->size());
        }
        socket->close();//?
        clearData();
    }
    else if(data.dev)
    {
        callWriteData = false;
        const qint64 blocksize = 16 * 1024;
        char buf[16*1024];
        qint64 read_num = data.dev->read(buf,blocksize);
#if defined (QFTPDTP_DEBUG)
        qDebug("QFtpDTP::writeData: wirte %d bytes",read_num);
#endif
        if(read_num > 0)
            socket->write(buf,read_num);
        else if(read_num == -1 || (!data.dev->isSequential() && data.dev->atEnd()))
            if(bytesDone == 0 && socket->bytesToWrite()== 0)
                emit dataTransferProgress(0,bytesTotal);
            socket->close();
            clearData();
    }
    // 是否继续上传
    callWriteData = (data.dev != 0);
}

bool QFtpDTP::hasError() const
{
    return !err.isNull();
}

QString QFtpDTP::errorMessage() const
{
    return err;
}

void QFtpDTP::cleanError()
{
    err.clear();
}

void QFtpDTP::socketConnected()
{
    bytesDone = 0;
#if defined (QFTPDTP_DEBUG)
    qDebug("QFtpDTP::connectState(QFtpDTP::CsConnected)");
#endif
    emit connectState(QFtpDTP::CsConnected);
}
void QFtpDTP::socketReadyRead()
{

}

void QFtpDTP::socketError(QAbstractSocket::SocketError e)
{
    if(e == QTcpSocket::HostNotFoundError)
    {
#if defined (QFTPDTP_DEBUG)
        qDebug("QFtp::connectState(QFtpDTP::CsHostNotFound)");
#endif
        emit connectState(QFtpDTP::CsHostNotFound);
    }
    else if(e == QTcpSocket::ConnectionRefusedError)
    {
#if defined (QFTPDTP_DEBUG)
        qDebug("QFtp::connectState(QFtpDTP::CsConnectRefused)");
#endif
        emit connectState(QFtpDTP::CsConnectRefused);
    }
}

void QFtpDTP::socketConnectionClosed()
{

}

void QFtpDTP::socketBytesWritten(qint64)
{

}

void QFtpDTP::setupSocket()
{

}

QFtp::QFtp(QObject *parent):QObject(parent)
{

}
QFtp::~QFtp()
{

}
