#include "ProcessThreadDlg.h"
#include "ui_ProcessThreadDlg.h"
#include <qdebug.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <qheaderview.h>

ProcessThreadDlg::ProcessThreadDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessThreadDlg)
{
    ui->setupUi(this);

    SetTableStyle();
}

ProcessThreadDlg::~ProcessThreadDlg()
{
    delete ui;
}

void ProcessThreadDlg::enumProcessThread()
{
    int rowCount = ui->tableWidget->rowCount();

    for( int i = 0; i < rowCount; i++ )
    {
        ui->tableWidget->removeRow(0);
    }

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 te = {sizeof(THREADENTRY32)};
    BOOL bSuccess = Thread32First(hSnap, &te);
    if (bSuccess==TRUE)
    {
        int i = 0;
        do
        {
            if (te.th32OwnerProcessID == m_nPid)
            {
                ui->tableWidget->insertRow(i);
                ui->tableWidget->setItem(i,0,new QTableWidgetItem(tr("%1").arg(te.th32ThreadID)));
                ui->tableWidget->setItem(i,1,new QTableWidgetItem(tr("%1").arg(te.tpBasePri)));
                i++;
            }
        } while (Thread32Next(hSnap, &te));
    }
    CloseHandle(hSnap);
}

void ProcessThreadDlg::SetTableStyle()
{
    setWindowTitle(QString::fromLocal8Bit("查看进程线程"));

    //设置选择行为，以行为单位
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //失去焦点
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    //不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //自适应宽度
    //自适应标题头
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

uint ProcessThreadDlg::getPid(uint pid)
{
    m_nPid = pid;
    return pid;
}
