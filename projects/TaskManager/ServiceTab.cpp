#include "ServiceTab.h"
#include "ui_ServiceTab.h"
#include <qdebug.h>
#include <TlHelp32.h>
#include <qheaderview.h>
#include <Windows.h>

ServiceTab::ServiceTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServiceTab)
{
    ui->setupUi(this);

    SetContextMenu();
    SetConnect();
    SetTableStyle();
    EnumServices();
}

ServiceTab::~ServiceTab()
{
    delete ui;
}

void ServiceTab::AddItem(ENUM_SERVICE_STATUS *pAry, DWORD lpServicesReturned)
{
    for (int i = 0; i < lpServicesReturned; i++) {
        //产生新行
        ui->tableWidget->insertRow(i);

        //名称
        QString serviceName = QString::fromWCharArray(pAry->lpServiceName);
        QTableWidgetItem* item1 = new QTableWidgetItem(serviceName);
        ui->tableWidget->setItem(i, 0, item1);

        //描述
        QString displayName = QString::fromWCharArray(pAry->lpDisplayName);
        QTableWidgetItem* item2 = new QTableWidgetItem(displayName);
        ui->tableWidget->setItem(i, 1, item2);

        //状态
        QTableWidgetItem* item3;

        switch(pAry->ServiceStatus.dwCurrentState){
        case SERVICE_PAUSED:
            item3 = new QTableWidgetItem(QString::fromLocal8Bit("暂停"));
            break;
        case SERVICE_STOPPED:
            item3 = new QTableWidgetItem(QString::fromLocal8Bit("停止"));
            break;
        case SERVICE_RUNNING:
            item3 = new QTableWidgetItem(QString::fromLocal8Bit("运行"));
            break;
        default:
            item3 = new QTableWidgetItem(QString::fromLocal8Bit("其他"));
            break;
        }

        ui->tableWidget->setItem(i, 2, item3);

        //向后偏移
        pAry++;
    }
}

void ServiceTab::EnumServices()
{
    //char szDisplayName[256] = { 0 };

    //char szServiceName[256] = { 0 };

    DWORD pcbBytesNeeded = 0;           // buffer size needed
    DWORD lpServicesReturned = 0;       // number of entries returned
    DWORD lpResumeHandle = 0;           // next entry


    //需要管理员权限
    schSCManager = OpenSCManager(
        NULL,                    // local machine
        NULL,                    // ServicesActive database
        SC_MANAGER_ALL_ACCESS);  // full access rights

    if (schSCManager == NULL){
        printf("OpenSCManager");
    }

    ENUM_SERVICE_STATUS ServiceStatus;
    //ServiceStatus.lpDisplayName = szDisplayName;
    //ServiceStatus.lpServiceName = szServiceName;
    BOOL bRet = FALSE;

    pcbBytesNeeded = 0;
    lpServicesReturned = 0;

    bRet = EnumServicesStatus(schSCManager,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        &ServiceStatus,
        0,
        &pcbBytesNeeded,
        &lpServicesReturned,
        &lpResumeHandle);

    pAry = NULL;
    ENUM_SERVICE_STATUS* pAryOrg = NULL;


    if (bRet == 0  && GetLastError() == ERROR_MORE_DATA) {
        //说明ServiceStatus空间不够
        pAry = (ENUM_SERVICE_STATUS*)new char[pcbBytesNeeded];
        pAryOrg = pAry;
    }


    bRet = EnumServicesStatus(schSCManager,
        SERVICE_WIN32,
        SERVICE_STATE_ALL,
        pAry,
        pcbBytesNeeded,
        &pcbBytesNeeded,
        &lpServicesReturned,
        &lpResumeHandle);

    AddItem(pAry, lpServicesReturned);

    delete[] pAryOrg;
}

void ServiceTab::SetTableStyle()
{
    //设置选择行为，以行为单位
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    //默许选中第一行
    //pTableWidget->selectRow(1);
    //pTableWidget->setFocus(Qt::MouseFocusReason);

    ui->tableWidget->setFixedWidth(800);
    ui->tableWidget->setFixedHeight(600);

    //自适应宽度
    //自适应标题头
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void ServiceTab::SetContextMenu()
{
    m_pContextMenu = new QMenu(this);
    setContextMenuPolicy(Qt::CustomContextMenu);
    m_pActStart = m_pContextMenu->addAction(QString::fromLocal8Bit("启动服务"));
    m_pActStop  = m_pContextMenu->addAction(QString::fromLocal8Bit("停止服务"));
}

void ServiceTab::SetConnect()
{
    //右键菜单
    QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(OnCustomContextMenuRequested(const QPoint &)));
    //启动服务
    QObject::connect(m_pActStart, SIGNAL(triggered()), this, SLOT(OnStartService()));
    //停止服务
    QObject::connect(m_pActStop, SIGNAL(triggered()),this,SLOT(OnStopService()));
}

void ServiceTab::OnCustomContextMenuRequested(const QPoint &pos)
{
    m_pContextMenu->exec(QCursor::pos());
}

QString getLastErrorMsg() {
    LPWSTR bufPtr = NULL;
    DWORD err = GetLastError();
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                   FORMAT_MESSAGE_FROM_SYSTEM |
                   FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, err, 0, (LPWSTR)&bufPtr, 0, NULL);
    //wchar* 转换为 QString
    const QString result = (bufPtr) ?
                            QString::fromUtf16((const ushort*)bufPtr).trimmed() :
                            QString("Unknown Error %1").arg(err);
    LocalFree(bufPtr);
    return result;
}

void ServiceTab::OnStartService()
{
    qDebug() <<"ServiceTab::OnStartService()" << endl;
    //获取选中行
    int row = ui->tableWidget->currentRow();

    //选中的服务本来是停止状态，不做修改
    if(pAry->ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        return;
    }

    //打开服务
    qDebug()<<row<<endl;
    QString name = ui->tableWidget->item(row, 0)->text();
    qDebug()<<name<<endl;
    LPCWSTR strVariable2 = (const wchar_t*) name.utf16();

    SC_HANDLE service = OpenService(schSCManager, strVariable2, SERVICE_ALL_ACCESS);
    if(service == nullptr)
    {
        qDebug() << QString::fromLocal8Bit("打开服务失败") << getLastErrorMsg();
        return;
    }

    //启动服务
    BOOL ret = StartService(service,0,nullptr);
    if(ret)
    {
        qDebug() << QString::fromLocal8Bit("启动服务成功");
        //设置服务状态提示
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::fromLocal8Bit("运行")));
        pAry->ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("启动服务失败");
    }
}

void ServiceTab::OnStopService()
{
    qDebug() <<"ServiceTab::OnStopService()" << endl;
    //获取选中行
    int row = ui->tableWidget->currentRow();

    //选中的服务本来是停止状态，不做修改
    if(pAry->ServiceStatus.dwCurrentState == SERVICE_STOPPED)
    {
        return;
    }

    //打开服务
    qDebug()<<row<<endl;
    QString name = ui->tableWidget->item(row, 0)->text();
    qDebug()<<name<<endl;
    LPCWSTR strVariable2 = (const wchar_t*) name.utf16();

    SC_HANDLE service = OpenService(schSCManager, strVariable2, SERVICE_ALL_ACCESS);
    if(service == nullptr)
    {
        qDebug() << QString::fromLocal8Bit("打开服务失败") << getLastErrorMsg();
        return;
    }

    //停止服务
    SERVICE_STATUS status;
    BOOL ret = ControlService(service,SERVICE_CONTROL_STOP,&status);
    if(ret)
    {
        qDebug() << QString::fromLocal8Bit("停止服务成功");
        //设置服务状态提示
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::fromLocal8Bit("停止")));
        pAry->ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("停止服务失败");
    }
}
