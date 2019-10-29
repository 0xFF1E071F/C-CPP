#include "RegEditTab.h"
#include "SpyppTab.h"
#include "TabDlg.h"

#include <qboxlayout.h>
#include <qstring.h>
#include <qtabwidget.h>
#include "ProcessTab.h"
#include "FileTab.h"
#include "ServiceTab.h"
#include "TraversalWndTab.h"

TabDlg::TabDlg(QWidget *parent) : QDialog(parent)
{
    m_pTabWidget = new QTabWidget;
    m_pTabWidget->addTab(new ProcessTab(),QString::fromLocal8Bit("进程"));
    m_pTabWidget->addTab(new FileTab(),QString::fromLocal8Bit("文件"));
    m_pTabWidget->addTab(new RegEditTab(),QString::fromLocal8Bit("注册表"));
    m_pTabWidget->addTab(new ServiceTab(),QString::fromLocal8Bit("服务"));
    m_pTabWidget->addTab(new TraversalWndTab(),QString::fromLocal8Bit("窗口"));
    m_pTabWidget->addTab(new SpyppTab(),QString::fromLocal8Bit("spy++"));

    SetTabStyle(m_pTabWidget);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(m_pTabWidget);
    setLayout(mainLayout);

    setWindowTitle(QString::fromLocal8Bit("我的任务管理器"));
}

void TabDlg::SetTabStyle(QTabWidget *pTabWidget)
{
    pTabWidget->setFixedWidth(822);
    pTabWidget->setFixedHeight(642);
}
