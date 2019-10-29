#ifndef TRAVERSALWNDTAB_H
#define TRAVERSALWNDTAB_H

#include <QStandardItemModel>
#include <QWidget>
#include <Windows.h>
#include <tchar.h>

namespace Ui {
class TraversalWndTab;
}

class TraversalWndTab : public QWidget
{
    Q_OBJECT

public:
    explicit TraversalWndTab(QWidget *parent = nullptr);
    ~TraversalWndTab();

    //设置表格属性
    void SetTreeViewStyle();
    //枚举窗口
    //void TraversalWndTab::addWindows();
    //枚举父窗口
    static BOOL CALLBACK StaticEnumWindowsProc(HWND hwnd, LPARAM lParam);
    BOOL EnumWindowsProc(HWND hwnd);
    //枚举子窗口
    static BOOL CALLBACK StaticEnumChildProc(HWND hwnd,LPARAM lParam);
    BOOL EnumChildProc(HWND hwnd);

private:
    void (TraversalWndTab::*pp)(void);

    Ui::TraversalWndTab *ui;

//    static QStandardItemModel *m_StdItemModel;
    QStandardItemModel *m_StdItemModel;

    QStandardItem* m_RootItem = nullptr;
    QStandardItem* m_ParentItem = nullptr;
    QStandardItem* m_ChildItem = nullptr;
    QList<QStandardItem*> m_ChildItems;
};



#endif // TRAVERSALWNDTAB_H
