#ifndef SERVICETAB_H
#define SERVICETAB_H

#include <QWidget>
#include <Windows.h>
#include <qmenu.h>

namespace Ui {
class ServiceTab;
}

class ServiceTab : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceTab(QWidget *parent = nullptr);
    ~ServiceTab();
    //向表格添加元素
    void AddItem(ENUM_SERVICE_STATUS* pAry, DWORD lpServicesReturned);
    void EnumServices();  //枚举服务
    void SetTableStyle(); //设置表格属性
    void SetContextMenu();//设置右键菜单
    void SetConnect();    //设置信号与槽

private:
    Ui::ServiceTab *ui;

    QMenu*   m_pContextMenu;//右键菜单
    QAction* m_pActStart;   //启动服务
    QAction* m_pActStop;    //停止服务

    //服务控制管理数据库句柄
    SC_HANDLE schSCManager;
    //枚举的服务指针
    ENUM_SERVICE_STATUS* pAry;

signals:

public slots:
    void OnCustomContextMenuRequested(const QPoint & pos);
    void OnStartService();
    void OnStopService();

};

#endif // SERVICETAB_H
