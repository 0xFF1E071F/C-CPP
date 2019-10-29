#include "FileTab.h"
#include "ui_FileTab.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>

FileTab::FileTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTab)
{
    ui->setupUi(this);

    SetFileUiStyle();
    LoadFileSystem();
}

FileTab::~FileTab()
{
    delete ui;
}

void FileTab::SetFileUiStyle()
{
    //设置窗口标题
    setWindowTitle("文件系统");

    //设置选择行为，以行为单位
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置选择模式，选择单行
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    //不可编辑
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //失去焦点
    ui->treeView->setFocusPolicy(Qt::NoFocus);

    //隐藏默认表头
//    ui->treeView->header()->hide();

    //------------- treeView -------------------------
    //------------- treeView -------------------------

    //------------- listView -------------------------
    //------------- listView -------------------------

    //------------- lineEdit -------------------------
    //------------- lineEdit -------------------------
}

void FileTab::LoadFileSystem()
{
//    m_StdItemModel = new QStandardItemModel(ui->treeView);
//    m_RootItem = new QStandardItem(QStringLiteral("我的电脑"));
//    m_StdItemModel->appendRow(m_RootItem);

//    ui->treeView->setModel(m_StdItemModel);

//    //展开根节点
//    QModelIndex indexRoot = m_StdItemModel->index(0, 0, QModelIndex());
//    ui->treeView->selectionModel();
////    ui->treeView->setRootIndex(indexRoot);
//    ui->treeView->expand(indexRoot);
//    ui->treeView->setRootIndex(m_FileSystemModel->index(QDir::currentPath()));

    m_FileSystemModel = new QFileSystemModel;
//    m_FileSystemModel->setRootPath(QDir::currentPath());
    m_FileSystemModel->setRootPath("");

    ui->treeView->setModel(m_FileSystemModel);

    ui->treeView->setColumnWidth(0, 400);//设置固定宽度

//------------------------------------------------------
//    foreach (QFileInfo my_info, QDir::drives())
//    {
//        qDebug()<<my_info.absoluteFilePath();
//    }

//------------------------------------------------------
//    QStorageInfo storage = QStorageInfo::root();

//    qDebug() << storage.rootPath();
//    if (storage.isReadOnly())
//    {
//        qDebug() << "isReadOnly:" << storage.isReadOnly();
//    }

//    qDebug() << "name:" << storage.name();
//    qDebug() << "fileSystemType:" << storage.fileSystemType();
//    qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
//    qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";

//------------------------------------------------------
//    QList<QStorageInfo> list = QStorageInfo::mountedVolumes();
//    qDebug() << "Volume Num: " << list.size();
//    for(QStorageInfo& si : list)
//    {
//        qDebug() << "Name: " << si.name();
//        qDebug() << "Block Size: " << si.blockSize();
//        qDebug() << "Bytes Avaliable: " << si.bytesAvailable();
//        qDebug() << "Bytes Free: " << si.bytesFree();
//        qDebug() << "Bytes Total: " << si.bytesTotal();
//        qDebug() << "Display Name: " << si.displayName();
//        qDebug() << "File System Type: " << si.fileSystemType();
//        qDebug() << "Is ReadOnly: " << si.isReadOnly();
//        qDebug() << "Is Root: " << si.isRoot();
//    }

//    qDebug() << QStorageInfo::root().device();
}
