#ifndef PROCESSTAB_H
#define PROCESSTAB_H

#include "ProcessModuleDlg.h"
#include "ProcessThreadDlg.h"

#include <QWidget>
#include <qmenu.h>
#include <qaction.h>
#include <qtablewidget.h>

class ProcessTab : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessTab(QWidget *parent = nullptr);
    void SetTitle();    //设置进程标题
    bool enumProcess(); //枚举进程
    int getPid();      //获取pid，用于发送到子窗口
    void upRole();      // 提升权限
    void SetTableStyle(QTableWidget* pTableWidget);


private:
    QTableWidget* m_pTableWidget;//进程

    //右键菜单
    QMenu* m_pContextMenu;       //菜单
    QAction *m_pActionViewThread;//查看线程
    QAction *m_pActionViewModule;//查看模块

    //右键查看模块的Dialog
    ProcessModuleDlg m_ProcessModuleDlg;
    //查看进程线程
    ProcessThreadDlg m_ProcessThreadDlg;

signals:
    void sendPid(uint pid);//向子窗口发送pid

public slots:
void OnCustomContextMenuRequested(const QPoint & pos);
void OnViewThread();
void OnViewModule();


};

#endif // PROCESSTAB_H
