#ifndef PROCESSMODULEDLG_H
#define PROCESSMODULEDLG_H

#include <QDialog>
#include <qtablewidget.h>

namespace Ui {
class ProcessModuleDlg;
}

class ProcessModuleDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessModuleDlg(QWidget *parent = nullptr);
    ~ProcessModuleDlg();
    //枚举进程模块
    void enumProcessModule();
    void SetTableStyle();

private:
    Ui::ProcessModuleDlg *ui;

    uint m_nPid;

signals:

public slots:
    //获取选中行的pid(父窗口信号通知)
    uint getPid(uint pid);
};

#endif // PROCESSMODULEDLG_H
