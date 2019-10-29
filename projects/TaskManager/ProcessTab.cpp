#include "ProcessTab.h"
#include "qdebug.h"

#include <QListWidget>
#include <qboxlayout.h>
#include <qtablewidget.h>
#include "TabDlg.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <qdialog.h>
#include <qheaderview.h>

ProcessTab::ProcessTab(QWidget *parent) : QWidget(parent)
{
    qDebug() << "ProcessTab(QWidget *parent) : QWidget(parent)"<<endl;
    m_pTableWidget = new QTableWidget;

    SetTableStyle(m_pTableWidget);

    m_pContextMenu = new QMenu(this);
    //右键菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    m_pActionViewThread = m_pContextMenu->addAction(QString::fromLocal8Bit("查看线程"));
    m_pActionViewModule = m_pContextMenu->addAction(QString::fromLocal8Bit("查看模块"));
    //链接信号槽
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(OnCustomContextMenuRequested(const QPoint &)));
    QObject::connect(m_pActionViewThread, SIGNAL(triggered()), this, SLOT(OnViewThread()));
    QObject::connect(m_pActionViewModule,SIGNAL(triggered()),this,SLOT(OnViewModule()));
    //进程模块，信号槽通知，传递pid
    QObject::connect(this, SIGNAL(sendPid(uint)), &m_ProcessModuleDlg, SLOT(getPid(uint)));
    QObject::connect(this, SIGNAL(sendPid(uint)), &m_ProcessThreadDlg, SLOT(getPid(uint)));

    SetTitle();
    enumProcess();
    upRole();//进程提权
}

void ProcessTab::SetTitle()
{
    //设置列数
    m_pTableWidget->setColumnCount(2);

    //设置标题
    QStringList header;
    header << QString::fromLocal8Bit("进程名") << QString::fromLocal8Bit("进程ID");
    m_pTableWidget->setHorizontalHeaderLabels(header);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(m_pTableWidget);
    setLayout(mainLayout);
}

bool ProcessTab::enumProcess()
{
    HANDLE hProcessSnap;
    BOOL bRet = FALSE;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcessSnap == INVALID_HANDLE_VALUE )
    {
        return bRet;
    }

    PROCESSENTRY32  pe32 = {0};
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(Process32First(hProcessSnap, &pe32))
    {
        int nRow = 0;
        do
        {
            std::wstring strName = pe32.szExeFile;
            QString str2 = QString::fromStdWString(strName);

            QString str = QString("pid: %1 name: %2").arg(str2).arg(pe32.th32ProcessID);
            //qDebug() << str << endl;

            m_pTableWidget->insertRow(nRow);
            QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg(str2));
            m_pTableWidget->setItem(nRow, 0, newItem);

            QTableWidgetItem *newItem2 = new QTableWidgetItem(tr("%1").arg(pe32.th32ProcessID));
            m_pTableWidget->setItem(nRow, 1, newItem2);

            ++nRow;
        }
        while(Process32Next(hProcessSnap, &pe32));
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    CloseHandle(hProcessSnap);
    return (bRet);
}

void ProcessTab::OnCustomContextMenuRequested(const QPoint &pos)
{
    m_pContextMenu->exec(QCursor::pos());
}

int ProcessTab::getPid()
{
    int row = m_pTableWidget->currentRow();
    if(row == -1)
    {
        return -1;
    }

    uint pid = m_pTableWidget->item(row, 1)->text().toUInt();

    emit sendPid(pid); //发送信号

    return pid;
}

void ProcessTab::upRole()
{
    HANDLE pHandle = GetCurrentProcess();
    HANDLE tHandle;
    // 打开当前进程的访问令牌
    BOOL ret = OpenProcessToken(pHandle,TOKEN_ALL_ACCESS,&tHandle);
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    // 获取描述权限的LUID，SE_DEBUG_NAME调试权限名字
    ret = LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid);
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    // 调整访问令牌的权限
    ret = AdjustTokenPrivileges(tHandle,FALSE,&tp,sizeof(tp),NULL,NULL);
    CloseHandle(tHandle);
}

void ProcessTab::SetTableStyle(QTableWidget *pTableWidget)
{
    //设置选择行为，以行为单位
    pTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    pTableWidget->setFocusPolicy(Qt::NoFocus);
    //默许选中第一行
    //pTableWidget->selectRow(1);
    //pTableWidget->setFocus(Qt::MouseFocusReason);

    //自适应宽度
    //自适应标题头
    pTableWidget->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    pTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //pTableWidget->setFixedWidth(800);
    //pTableWidget->setFixedHeight(600);
}

void ProcessTab::OnViewThread()
{
    if(getPid() != -1)
    {
        m_ProcessThreadDlg.enumProcessThread();
        m_ProcessThreadDlg.exec();
    }
}

void ProcessTab::OnViewModule()
{
    if(getPid() != -1)
    {
        m_ProcessModuleDlg.enumProcessModule();
        m_ProcessModuleDlg.exec();
    }
}


