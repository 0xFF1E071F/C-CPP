#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>

class QWidget;
class QTabWidget;

//自定义Tab对话框
class TabDlg : public QDialog
{
public:
    TabDlg(QWidget *parent = 0);
    void SetTabStyle(QTabWidget * pTabWidget);

private:
    QTabWidget *m_pTabWidget;
};

#endif // TABDIALOG_H
