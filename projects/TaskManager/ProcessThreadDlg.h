#ifndef PROCESSTHREADDLG_H
#define PROCESSTHREADDLG_H

#include <QDialog>

namespace Ui {
class ProcessThreadDlg;
}

class ProcessThreadDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessThreadDlg(QWidget *parent = nullptr);
    ~ProcessThreadDlg();

    //枚举进程线程
    void enumProcessThread();
    void SetTableStyle();

private:
    Ui::ProcessThreadDlg *ui;
    uint m_nPid;

signals:

public slots:
    //获取选中行的pid(父窗口信号通知)
    uint getPid(uint pid);
};

#endif // PROCESSTHREADDLG_H
