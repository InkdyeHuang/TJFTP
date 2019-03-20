#ifndef TJFTP_H
#define TJFTP_H

#include <QMainWindow>
#include "widget_item.h"
class TabWidget;
class TJFTP : public QMainWindow
{
    Q_OBJECT

public:
    TJFTP(QWidget *parent=0);
    ~TJFTP();
private:
    QSplitter *hSplitter;
    QToolBar *userInfoToolBar;
    QToolBar * addressInfoToolBar;
    QLabel *userNameLabel;
    QComboBox *userNameComboBox;
    QLabel *passwordLable;
    QLineEdit *passwordLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QLabel *addressLabel;
    QComboBox *addressComboBox;
    QPushButton *goPushButton;
    QCheckBox *anonymousCheckBox;
    TabWidget *remoteDirTabWidget;
    TabWidget *localDirTabWideget;
};

#endif // TJFTP_H
