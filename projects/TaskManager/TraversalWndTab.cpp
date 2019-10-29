#include "TraversalWndTab.h"
#include "ui_TraversalWndTab.h"
#include <iostream>
#include <qdebug.h>

TraversalWndTab::TraversalWndTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TraversalWndTab)
{
    ui->setupUi(this);

    m_StdItemModel = new QStandardItemModel(ui->treeView);
    //m_StdItemModel->setHorizontalHeaderLabels("");

    SetTreeViewStyle();

    EnumWindows(StaticEnumWindowsProc, reinterpret_cast<LPARAM>(this));

//    QModelIndex index = m_StdItemModel->index(1, 0);
//    ui->treeView->expand(index);

    ui->treeView->setModel(m_StdItemModel);

    //展开根节点
    QModelIndex indexRoot = m_StdItemModel->index(0, 0, QModelIndex());
    ui->treeView->selectionModel();
//    ui->treeView->setRootIndex(indexRoot);
    ui->treeView->expand(indexRoot);
}

TraversalWndTab::~TraversalWndTab()
{
    delete ui;
}

void TraversalWndTab::SetTreeViewStyle()
{
    //设置选择行为，以行为单位
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->treeView->setFocusPolicy(Qt::NoFocus);

    //外框宽高
    ui->treeView->setFixedWidth(800);
    ui->treeView->setFixedHeight(600);

    //隐藏默认表头
    ui->treeView->header()->hide();
}

BOOL TraversalWndTab::StaticEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    TraversalWndTab* pThis = reinterpret_cast<TraversalWndTab*>(lParam);
    return pThis->EnumWindowsProc(hwnd);
}

QStandardItem* m_TempParentItem = nullptr;

BOOL TraversalWndTab::EnumWindowsProc(HWND hwnd)
{
    TCHAR szTitle[MAX_PATH] = { 0 };
    TCHAR szClass[MAX_PATH] = { 0 };
    DWORD dwProcessId = 0;
    int nMaxCount = MAX_PATH;

    LPTSTR  lpWindowName = szTitle;
    LPTSTR  lpClassName = szClass;

    GetWindowText(hwnd, lpWindowName, nMaxCount);
    GetClassName(hwnd, lpClassName, nMaxCount);
    GetWindowThreadProcessId(hwnd, &dwProcessId);

    //qDebug() << "parent window" << hwnd << QString::fromWCharArray(lpWindowName) << QString::fromWCharArray(lpClassName) << endl;

    if(m_RootItem == nullptr)
    {
        m_RootItem = new QStandardItem(QStringLiteral("窗口列表"));
        m_StdItemModel->appendRow(m_RootItem);
//        qDebug() << m_RootItem << endl;
    }

    QString str = QString(QString::fromLocal8Bit("窗口 %1 \"%2\" %3")).
            arg(QString::number(reinterpret_cast<std::uintptr_t>(hwnd),16), 8, QLatin1Char('0')).
            arg(QString::fromWCharArray(lpWindowName)).
            arg(QString::fromWCharArray(lpClassName));

    m_ParentItem = new QStandardItem(str);
    m_RootItem->appendRow(m_ParentItem);
//    qDebug() << m_ParentItem <<endl;

    m_TempParentItem = m_ParentItem;

    EnumChildWindows(hwnd, StaticEnumChildProc, NULL);

//    ui->treeView->setModel(m_StdItemModel);
//    ui->treeView->setRootIndex();

    return TRUE;
}

BOOL TraversalWndTab::StaticEnumChildProc(HWND hwnd, LPARAM lParam)
{
    TraversalWndTab* pThis = reinterpret_cast<TraversalWndTab*>(lParam);
    return pThis->EnumChildProc(hwnd);
}

BOOL TraversalWndTab::EnumChildProc(HWND hwnd)
{
//    qDebug() << "TraversalWndTab::EnumChildProc(HWND hwnd)"<< m_TempParentItem << endl;
    TCHAR szTitle[MAX_PATH] = { 0 };
    TCHAR szClass[MAX_PATH] = { 0 };
    DWORD dwProcessId = 0;
    int nMaxCount = MAX_PATH;

    LPTSTR  lpWindowName = szTitle;
    LPTSTR  lpClassName = szClass;

    GetWindowText(hwnd, lpWindowName, nMaxCount);
    GetClassName(hwnd, lpClassName, nMaxCount);
    GetWindowThreadProcessId(hwnd, &dwProcessId);

    QString str = QString(QString::fromLocal8Bit("窗口 %1 \"%2\" %3")).
            arg(QString::number(reinterpret_cast<std::uintptr_t>(hwnd),16), 8, QLatin1Char('0')).
            arg(QString::fromWCharArray(lpWindowName)).
            arg(QString::fromWCharArray(lpClassName));
    QStandardItem* item2 = new QStandardItem(str);
    m_TempParentItem->appendRow(item2);

    return TRUE;
}

//BOOL TraversalWndTab::EnumWindowsProcStatic(HWND hwnd, LPARAM lParam)
//{
//    TraversalWndTab* p = reinterpret_cast<TraversalWndTab*>(lParam);
//    return p->EnumWindowsProc(hwnd);
//}

//BOOL TraversalWndTab::EnumWindowsProc(HWND hwnd, LPARAM lParam)
//{
//    TCHAR szTitle[MAX_PATH] = { 0 };
//        TCHAR szClass[MAX_PATH] = { 0 };
//        DWORD dwProcessId = 0;
//        int nMaxCount = MAX_PATH;

//        LPTSTR  lpWindowName = szTitle;
//        LPTSTR  lpClassName = szClass;

//        //HWND hWnd = GetDesktopWindow();

//        GetWindowText(hwnd, lpWindowName, nMaxCount);
//        GetClassName(hwnd, lpClassName, nMaxCount);
//        GetWindowThreadProcessId(hwnd, &dwProcessId);

//        wcout << "[Parent window] window hanlde: " << hwnd
//            << " window name: " << lpWindowName
//            << " class name: " << lpClassName
//            << " process id: " << dwProcessId
//            << endl;
//        //qDebug() << "parent window" << hwnd << QString::fromWCharArray(lpWindowName) << QString::fromWCharArray(lpClassName) << endl;
//        //插入父节点

//        QList<QStandardItem*> items1;
//        QString str = QString(tr("窗口 %1%2%3")).
//                arg(QString::number((std::uintptr_t)(hwnd))).
//                arg(QString::fromWCharArray(lpWindowName)).
//                arg(QString::fromWCharArray(lpClassName));
//        QStandardItem* item1 = new QStandardItem(str);
//        items1.append(item1);
//        m_StdItemModel->appendRow(items1);

//        //SetStaticUiModel();
//        //ui->treeview ->setModel(m_StdItemModel)

//        //if (GetParent(hwnd) == NULL && IsWindowVisible(hwnd))  //判断是否顶层窗口并且可见
//        //{
//            EnumChildWindows(hwnd, EnumChildProc, lParam);
//        //}

//        return TRUE;
//}

//BOOL TraversalWndTab::EnumChildProc(HWND hwnd, LPARAM lParam)
//{
//    TCHAR szTitle[MAX_PATH] = { 0 };
//        TCHAR szClass[MAX_PATH] = { 0 };
//        DWORD dwProcessId = 0;
//        int nMaxCount = MAX_PATH;

//        LPTSTR  lpWindowName = szTitle;
//        LPTSTR  lpClassName = szClass;

//        GetWindowText(hwnd, lpWindowName, nMaxCount);
//        GetClassName(hwnd, lpClassName, nMaxCount);
//        GetWindowThreadProcessId(hwnd, &dwProcessId);

//        wcout << "[Child window] window handle: " << hwnd
//            << " window name: " << lpWindowName
//            << " class name: " << lpClassName
//            << " process id: " << dwProcessId
//            << endl;

//        //qDebug() << "child window" << hwnd << QString::fromWCharArray(lpWindowName) << QString::fromWCharArray(lpClassName) << endl;
//        //窗口 窗口句柄 窗口标题 类名
//        //插入子节点



//        return TRUE;
//}
