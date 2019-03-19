#include "tjftp.h"
#include "tabwidget.h"
#include "remotedirwidget.h"
#include "localdirwidget.h"
TJFTP::TJFTP(QWidget *parent)
    : QMainWindow(parent)
{
    hSplitter = new QSplitter(Qt::Horizontal,this);

    userNameLabel = new QLabel(tr("用户名:"),this);
    userNameComboBox = new QComboBox(this);
    userNameComboBox->setEditable(true);
    userNameComboBox->setFixedWidth(100);

    passwordLable = new QLabel(tr("口令:"),this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setFixedWidth(100);

    portLabel = new QLabel(tr("口令："),this);
    portLineEdit = new QLineEdit(tr("21"),this);
    portLineEdit->setFixedWidth(50);
    portLineEdit->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);

    anonymousCheckBox = new QCheckBox(tr("匿名"),this);

    addressLabel = new QLabel(tr("地址"),this);
    addressComboBox = new QComboBox(this);
    addressComboBox->setEditable(true);
    addressComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    goPushButton = new QPushButton(tr("转到"),this);

    userInfoToolBar = addToolBar(tr("用户信息"));
    userInfoToolBar->addWidget(userNameLabel);
    userInfoToolBar->addWidget(userNameComboBox);
    userInfoToolBar->addSeparator();
    userInfoToolBar->addWidget(passwordLable);
    userInfoToolBar->addWidget(passwordLineEdit);
    userInfoToolBar->addSeparator();
    userInfoToolBar->addWidget(portLabel);
    userInfoToolBar->addWidget(portLineEdit);
    userInfoToolBar->addSeparator();
    userInfoToolBar->addWidget(anonymousCheckBox);
    userInfoToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    QSplitter * splitter = new QSplitter(Qt::Horizontal,this);
    splitter->addWidget(addressLabel);
    splitter->addWidget(addressComboBox);
    splitter->addWidget(goPushButton);
    splitter->setStretchFactor(1,1);
    addressInfoToolBar = addToolBar(tr("地址信息"));
    addressInfoToolBar->addWidget(splitter);
    addressInfoToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    localDirTabWideget = new TabWidget(this);
    localDirTabWideget->setEnableMutiTab(false);
    localDirTabWideget->addTab(new LocalDirWidget(this),tr("本地"));

    remoteDirTabWidget = new TabWidget(this);
    remoteDirTabWidget->setEnableMutiTab(true);
    remoteDirTabWidget->addTab(new RemoteDirWidget(this),tr(" "));
    hSplitter->addWidget(localDirTabWideget);
    hSplitter->addWidget(remoteDirTabWidget);
    setCentralWidget(hSplitter);


    setWindowTitle(tr("TJFTP"));
}

TJFTP::~TJFTP()
{

}
