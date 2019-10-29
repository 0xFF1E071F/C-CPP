#ifndef FILETAB_H
#define FILETAB_H

#include <QWidget>
#include <qfilesystemmodel.h>
#include <qstandarditemmodel.h>

namespace Ui {
class FileTab;
}

class FileTab : public QWidget
{
    Q_OBJECT

public:
    explicit FileTab(QWidget *parent = nullptr);
    ~FileTab();

    //设置组件属性
    void SetFileUiStyle();
    //加载系统盘符
    void LoadFileSystem();

private:
    Ui::FileTab *ui;
    QFileSystemModel * m_FileSystemModel;
    QStandardItemModel *m_StdItemModel;
    QStandardItem* m_RootItem;
};

#endif // FILETAB_H
