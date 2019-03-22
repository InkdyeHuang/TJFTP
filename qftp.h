#ifndef QFTP_H
#define QFTP_H

#include<QtCore/qstring.h>
#include<QtCore/qobject.h>
#include "qurlinfo.h"

class QFtpPrivate;

class QFtp:public QObject
{
    Q_OBJECT
public:
    explicit QFtp(QObject *parent = 0);
    virtual ~QFtp();
    enum State{
        Unconected,
        HostLookup,
        Connecting,
        Connected,
        LoggedIn,
        Closing
    };
    enum Error{
        NoError,
        UnkownError,
        HostNotFound,
        ConnectionRefused,
        NotConnected
    };
    enum Command{
        None,
        SetTransferMode,
        SetProxy,
        ConnectToHost,
        Login,
        Close,
        List,
        Cd,
        Get,
        Put,
        Remove,
        Mkdir,
        Rmdir,
        Rename,
        RawCommand
    };
    enum TransferMode{
        Active,
        Passive
    };
    enum TransferType{
        Binary,
        Ascii
    };
};

#endif // QFTP_H
