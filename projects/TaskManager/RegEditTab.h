#ifndef REGEDITTAB_H
#define REGEDITTAB_H

#include "RegEditModDlg.h"

#include <QWidget>
#include <qevent.h>
#include <qstandarditemmodel.h>
#include <qstringlistmodel.h>
#include <qtablewidget.h>
#include <qtreeview.h>

namespace Ui {
class RegEditTab;
}

//event filter

class RegEditTab : public QWidget
{
    Q_OBJECT

public:
    explicit RegEditTab(QWidget *parent = nullptr);
    ~RegEditTab();

    //信号槽
    void SetConnect();

    //设置ui属性
    void SetUiStyle();
    //初始treeview（5项根目录）
    void InitReg();

    //遍历
    void iterateOverItems();
    QList<QStandardItem*> returnTheItems();
    //双击
    void mouseDoubleClickEvent(QMouseEvent *event);

    //添加childGroups
    void addChildGroups(QString strPath, QStandardItem *parent);
    //添加childKeys
    void addChildKeys();
    void addChildKeysData(QString str);

    //读注册表
    //void ReadSettings();
    //void addChildSettings(QSettings &settings, Q);
    void test();

    //获取绝对路径
    //QString getAbsolutePath(QStandardItem * item);
    QString getAbsolutePath(QModelIndex itemIndex);

    //替换"HKEY_CURRENT_CONFIG" 需要替换
    //HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current
    QString replaceHCC(QString str);

    //contextMenuPolicy
    void SetCustomContextMenu();

    //tableWidget
    void createAction();      //create action
    void createMenu();        //create menu
    void createContextMenu(); //contextMenuPolicy
    void setMenuStatus();     //set menu status
    //create default regedit string value
    void createDefaultRegStringValue();
    //setting regedit new string value
    void setRegNewStringValue(QString str);
    void setRegNewStringValueKey();
    //key press response
    void keyPressEvent(QKeyEvent * event);
    //--tableWidget

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::RegEditTab *ui;

    //listview model
    QStandardItemModel *m_lsvModel;
    QStringListModel *model;
    //treeview model
    QStandardItemModel *m_StdItemModel;

    //tableWidget menu
    QMenu *m_pTableWidgetMenu; //noValid
    QMenu *m_pActNewMenu;      //menu 新建
    QAction *m_pActStringValue;//-- 字符串值

    QMenu *m_pTableWgtMenuValid;//isValid
    QAction *m_pActModify;      //modify
    QAction *m_pActDelete;      //delete
    QAction *m_pActRename;      //rename

    //modify regedit
    RegEditModDlg *regeditMod;
    //is it possible to modify it
    bool editFlag = false;
    //save regedit old key
    QString strOldKey;

signals:
    //update tableWidget ui
//    void updateTableWSig();

public slots:
    //tableWidget context menu
//    void OnTableWidgetCustomContextMenuRequested(const QPoint &pos);
    void OnTableWCustomContextMenuRequested(const QPoint &pos);

    // create regedit string value
    void createRegStringValue();

    //valid modify
    void modifyRegSlot();

    //rename regedit key
    void renameRegSlot();

    //delete regedit key-value
    void deleteRegSlot();

    //receiving subwindow data
    void receiveData(QString str);

    //modify regedit value
    void receiveModData(QString str);

    //item changed
    void itemChangedSlot(QTableWidgetItem *item);

    //item entered
    void itemEnteredSlot(QTableWidgetItem *item);

    // cell double clicked slot
    void cellDoubleClickedSlot(int row,int column);

    //
    void currentCellChangedSlot(int currentRow, int currentColumn, int previousRow, int previousColumn);
    //--tableWidget context menu

    //treeview 单击
    void OnTreeViewClick(const QModelIndex & index);
    //treeview 双击
    void OnTreeViewDoubleClick(const QModelIndex & index);

};

#endif // REGEDITTAB_H
