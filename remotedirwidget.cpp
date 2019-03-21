#include "remotedirwidget.h"
#include "remotedirtreeview.h"
#include "dirtreemodel.h"
RemoteDirWidget::RemoteDirWidget(QWidget *parent):QWidget (parent)
{
    parentTJFTP = reinterpret_cast<TJFTP *>(parent);

    remoteDirTreeModel = new DirTreeModel(this);
    remoteDirTreeView = new RemoteDirTreeView(this);
    remoteDirTreeView->setModel(remoteDirTreeModel);
    remoteDirTreeView->header()->setStretchLastSection(true);
    remoteDirTreeView->resizeColumnsToContents();
    remoteDirTreeView->setAlternatingRowColors(true);
    remoteDirTreeView->setSelectionMode(QAbstractItemView::ContiguousSelection);
    remoteDirTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    remoteDirTreeView->setSortingEnabled(true);
    remoteDirTreeView->sortByColumn(0,Qt::AscendingOrder);
    remoteDirTreeView->setItemsExpandable(false);
    remoteDirTreeView->setRootIsDecorated(false);
    remoteDirTreeView->setExpandsOnDoubleClick(false);
    remoteDirTreeView->setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked);

    remoteDirFileSystemModel = new QFileSystemModel(this);

    remoteDirComboBox = new QComboBox(this);

    connectToolButton = new QToolButton(this);
    connectToolButton->setText(tr("连接"));
    dotdotToolButton = new QToolButton(this);
    dotdotToolButton->setText(tr("上级目录"));
    refreshToolButton = new QToolButton(this);
    refreshToolButton->setText(tr("刷新"));

    logTextEdit = new QTextEdit(this);
    logTextEdit->setFixedHeight(100);
    logTextEdit->setReadOnly(true);

    remoteDirStatusBar = new QStatusBar(this);

    QHBoxLayout *topHBoxLayout = new QHBoxLayout;
    topHBoxLayout->addWidget(connectToolButton);
    topHBoxLayout->addWidget(dotdotToolButton);
    topHBoxLayout->addWidget(refreshToolButton);
    topHBoxLayout->addWidget(remoteDirComboBox);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topHBoxLayout);
    mainLayout->addWidget(remoteDirTreeView);
    mainLayout->addWidget(logTextEdit);
    mainLayout->addWidget(remoteDirStatusBar);
    mainLayout->setStretch(1,1);
    setLayout(mainLayout);

}

RemoteDirWidget::~RemoteDirWidget()
{

}
