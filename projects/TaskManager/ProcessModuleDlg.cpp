#include "ProcessModuleDlg.h"
#include "ui_ProcessModuleDlg.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <qdebug.h>
#include <qheaderview.h>

ProcessModuleDlg::ProcessModuleDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessModuleDlg)
{
    ui->setupUi(this);

    SetTableStyle();
}

ProcessModuleDlg::~ProcessModuleDlg()
{
    delete ui;
}

uint ProcessModuleDlg::getPid(uint pid)
{
    m_nPid = pid;
    return pid;
}

void ProcessModuleDlg::enumProcessModule()
{
    // 清空表格
    int rowCount = ui->tableWidget->rowCount();

    for( int i = 0; i < rowCount; i++ )
    {
        ui->tableWidget->removeRow(0);
    }

    qDebug() << "ProcessModuleDlg::enumProcessModule()" << m_nPid << endl;

    HANDLE snapHandele = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPTHREAD , m_nPid);
    if( INVALID_HANDLE_VALUE == snapHandele)
    {
        qDebug() << "CreateToolhelp32Snapshot error";
        return;
    }
    MODULEENTRY32 entry = {0};
    entry.dwSize = sizeof(entry);// 长度必须赋值
    BOOL ret = Module32First(snapHandele,&entry);
    int i = 0;
    while (ret) {
        QString dllFile = QString::fromWCharArray(entry.szModule);
        QString dllPath = QString::fromWCharArray(entry.szExePath);
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(dllFile));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString("%1").arg(dllPath)));
        i++;
        ret = Module32Next(snapHandele,&entry);
    }
    CloseHandle(snapHandele);
}

void ProcessModuleDlg::SetTableStyle()
{
    setWindowTitle(QString::fromLocal8Bit("查看进程模块"));

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
