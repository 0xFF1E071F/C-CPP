#include "RegEditTab.h"
#include "ui_RegEditTab.h"

#include <QSettings>
#include <QStringListModel>
#include <qstandarditemmodel.h>
#include <qdebug.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <Windows.h>
//#include <qt_windows.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <QMenu>
#include <qdialogbuttonbox.h>

RegEditTab::RegEditTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegEditTab)
{
    ui->setupUi(this);
    //        test()
    //regedit dlg
    regeditMod = new RegEditModDlg(this);
    //后记，如果这里不写this，cellDoubleClicked的时候，程序窗口会跑到其他程序窗口的后面
//    regeditMod->deleteLater();

//    ui->tableWidget->setFocusPolicy(Qt::StrongFocus);
    ui->tableWidget->installEventFilter(this);

    createMenu();
    createAction();
    createContextMenu();

    SetConnect();
    SetUiStyle();
    InitReg();
}

RegEditTab::~RegEditTab()
{
    delete ui;
}

void RegEditTab::SetConnect()
{
    //tableWidget context menu
    QObject::connect(ui->tableWidget,
                     SIGNAL(customContextMenuRequested(const QPoint &)),
                     this,
                     SLOT(OnTableWCustomContextMenuRequested(const QPoint &)));

    //itemDoubleClicked(QTableWidgetItem *item)
    QObject::connect(ui->tableWidget,
                     SIGNAL(itemDoubleClicked(QTableWidgetItem *item)),
                     this,
                     SLOT(OnTableWCustomContextMenuRequested(const QPoint &)));

    connect(ui->tableWidget,
            SIGNAL(cellDoubleClicked(int,int)),
            this,
            SLOT(cellDoubleClickedSlot(int,int)));

    //create regedit string value
    QObject::connect(m_pActStringValue,
                     SIGNAL(triggered()),
                     this,
                     SLOT(createRegStringValue()));
    //valid modify
    QObject::connect(m_pActModify,
                     SIGNAL(triggered()),
                     this,
                     SLOT(modifyRegSlot()));

    //rename regedit key
    QObject::connect(m_pActRename,
                     SIGNAL(triggered()),
                     this,
                     SLOT(renameRegSlot()));

    //delete regedit key-value
    QObject::connect(m_pActDelete,
                     SIGNAL(triggered()),
                     this,
                     SLOT(deleteRegSlot()));

    //receiving subwindow data
    QObject::connect(regeditMod,
                     SIGNAL(sendData(QString)),
                     this,
                     SLOT(receiveData(QString)));
    //receiving subwindow data, just to modify regedit value
    QObject::connect(regeditMod,
                     SIGNAL(sendModDataSig(QString)),
                     this,
                     SLOT(receiveModData(QString)));

    //item changed
    QObject::connect(ui->tableWidget,
                     SIGNAL(itemChanged(QTableWidgetItem *)),
                     this,
                     SLOT(itemChangedSlot(QTableWidgetItem*)));

    //item entered
    QObject::connect(ui->tableWidget,
                     SIGNAL(itemEntered(QTableWidgetItem *)),
                     this,
                     SLOT(itemEnteredSlot(QTableWidgetItem*)));

    QObject::connect(ui->tableWidget,
                     SIGNAL(currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)),
                     this,
                     SLOT(currentCellChangedSlot(int currentRow, int currentColumn, int previousRow, int previousColumn)));

    //--tableWidget context menu

    //treeview item单击
    QObject::connect(ui->treeView,
                     SIGNAL(clicked(const QModelIndex)),
                     this,
                     SLOT(OnTreeViewClick(const QModelIndex)));

    //treeview item双击
    QObject::connect(ui->treeView,
                     SIGNAL(doubleClicked ( const QModelIndex)),
                     this,
                     SLOT(OnTreeViewDoubleClick(const QModelIndex)));
}

void RegEditTab::SetUiStyle()
{
    //设置窗口标题
    setWindowTitle("注册表");

    //------------- treeView -------------------------
    //设置选择行为，以行为单位
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->treeView->setFocusPolicy(Qt::NoFocus);
    //隐藏默认表头
    ui->treeView->header()->hide();
    //设置treewidget水平滚动条
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->header()->setStretchLastSection(false);

    //------------- treeView -------------------------

    //------------- tableWidget -------------------------
    //设置选择行为，以行为单位
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);

    //自适应标题头
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //------------- tableWidget -------------------------

    //------------- lineEdit -------------------------
    ui->lineEdit->setReadOnly(true);
    //------------- lineEdit -------------------------
}

void RegEditTab::InitReg()
{
    //treeview
    m_StdItemModel = new QStandardItemModel(ui->treeView);

    QStandardItem *RootItem = new QStandardItem(QStringLiteral("我的电脑"));
    m_StdItemModel->appendRow(RootItem);

    QStandardItem *item1 = new QStandardItem(QStringLiteral("HKEY_CLASSES_ROOT"));
    QStandardItem *item2 = new QStandardItem(QStringLiteral("HKEY_CURRENT_USER"));
    QStandardItem *item3 = new QStandardItem(QStringLiteral("HKEY_LOCAL_MACHINE"));
    QStandardItem *item4 = new QStandardItem(QStringLiteral("HKEY_USERS"));
    QStandardItem *item5 = new QStandardItem(QStringLiteral("HKEY_CURRENT_CONFIG"));

    RootItem->appendRow(item1);
    RootItem->appendRow(item2);
    RootItem->appendRow(item3);
    RootItem->appendRow(item4);
    RootItem->appendRow(item5);

    ui->treeView->setModel(m_StdItemModel);

    //展开根节点
    QModelIndex indexRoot = m_StdItemModel->index(0, 0, QModelIndex());
    ui->treeView->selectionModel();
    ui->treeView->expand(indexRoot);
}

void RegEditTab::iterateOverItems()
{
    QList<QStandardItem*> list = returnTheItems();

    foreach (QStandardItem* item, list) {
        qDebug() << item->text();
    }
}

QList<QStandardItem *> RegEditTab::returnTheItems()
{
    return m_StdItemModel->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive);
}

void RegEditTab::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QModelIndex index0 = ui->treeView->currentIndex();
        qDebug() << index0.data().toString();
    }
}

void RegEditTab::addChildGroups(QString strPath, QStandardItem *parent)
{
    //ui->treeView->setFrameShape(QFrame::NoFrame);
    QSettings reg(strPath, QSettings::NativeFormat);
    QStringList groupsList = reg.childGroups();

    foreach(QString group,groupsList)
    {
        reg.beginGroup(group);

        parent->appendRow(new QStandardItem(group));
        reg.endGroup();
    }
    ui->treeView->setModel(m_StdItemModel);
}

void RegEditTab::addChildKeys()
{
    //设置lineEdit   ...可以适当封装 ----------
    //选中的行
    QModelIndex selectedIndex = ui->treeView->currentIndex();

    //获取选中行数据
    QString strPath = selectedIndex.data().toString();
    //qDebug() << strPath << endl;

    //当前节点
    QStandardItem* currentItem = m_StdItemModel->itemFromIndex(selectedIndex);
    //获取绝对路径
    QString strAbsolutePath = getAbsolutePath(selectedIndex);
    //    qDebug() << "getAbsolutePath" << strAbsolutePath;
    //设置lineEdit的text
    ui->lineEdit->setText(strAbsolutePath);
    //--lineEdit   ----------------

    //遍历并添加childKeys
    //添加到tableWidget
    addChildKeysData(strAbsolutePath);
    //--childKeys
}


const char *predef_key_names[] = {
    "HKEY_LOCAL_MACHINE", "HKEY_CLASSES_ROOT",
    "HKEY_CURRENT_USER", "HKEY_CURRENT_CONFIG",
    "HKEY_USERS"
};

HKEY predef_keys[] = {
    HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT,
    HKEY_CURRENT_USER, HKEY_CURRENT_CONFIG,
    HKEY_USERS
};

std::vector<std::string> key_names_v(predef_key_names,
                           predef_key_names+5);

const std::string predef_value_type_names[] = {
    "REG_SZ", "REG_EXPAND_SZ",
    "REG_MULTI_SZ", "REG_BINARY",
    "REG_DWORD", "REG_QWORD",
    "REG_DWORD_BIG_ENDIAN",
    "REG_LINK", "REG_NONE",
    "REG_RESOURCE_LIST"
};

std::vector<std::string> value_type_names(predef_value_type_names,
                                          predef_value_type_names+10);

DWORD predef_value_types[] = {
    REG_SZ, REG_EXPAND_SZ,
    REG_MULTI_SZ, REG_BINARY,
    REG_DWORD, REG_QWORD,
    REG_DWORD_BIG_ENDIAN,
    REG_LINK, REG_NONE,
    REG_RESOURCE_LIST
};

std::vector<DWORD> value_types_v(predef_value_types,
                                 predef_value_types+12);

inline void parse_value_type(std::string &type_string, DWORD value_type)
{
    for(int i = 0; i != static_cast<int>(value_types_v.size()); i++)
    {
        if(value_type == value_types_v[i])
        {
            type_string = predef_value_type_names[i];
            return;
        }
    }
    type_string = "UNKNOWN";
}

inline void parse_value_string(std::string &value_string,
                               DWORD value_type,
                               LPBYTE value_buff,
                               DWORD value_len)
{
    std::ostringstream ss;
    int n = 0;
    int len = 0;
    int count = 0;
    const char *ptr = nullptr;

    switch (value_type)
    {
    case REG_SZ:
    case REG_EXPAND_SZ:
    case REG_RESOURCE_LIST:
    {
        value_buff[value_len] = '\0';
        //LPBYTE 转换为 wchar_t*
        wchar_t * pWideStr = reinterpret_cast<wchar_t*>(value_buff);
        //wchar_t* 转换为 QString
        QString pQstr = QString::fromWCharArray(pWideStr);
//        qDebug() << "pQstr " << pQstr << '\n';
        //QString 转换为 std::string
        std::string stdStr = pQstr.toStdString();
//        qDebug() << "stdStr " << pQstr << '\n';
        value_string = stdStr;
        break;
    }

    case REG_DWORD:
    {
        ss.clear();
        ss << *(int*)value_buff;
        value_string = ss.str();
        break;
    }

    case REG_DWORD_BIG_ENDIAN:
    {
        ss.clear();
        ss << (int)((value_buff[3]<<0)|(value_buff[2]<<8)|
                    (value_buff[1]<<16)|(value_buff[0]<<24));
        value_string = ss.str();
        break;
    }

    case REG_QWORD:
    {
        ss.clear();
        ss << *(long long*)value_buff;
        value_string = ss.str();
        break;
    }

    case REG_BINARY:
    case REG_NONE:
    {
        ss.clear();
        for(int i = 0; i < (int)value_len; i++)
        {
            ss << "\\\\x";
            n = value_buff[i];
            ss << std::hex << std::setw(2) <<std::setfill('0') << n;
            ss << " ";
        }
        value_string = ss.str();
        qDebug() << QString::fromStdString(value_string) << '\n';
        break;
    }

    case REG_MULTI_SZ:
    {
//        //LPBYTE 转换为 wchar_t*
//        wchar_t * pWideStr = reinterpret_cast<wchar_t*>(value_buff);
//        //wchar_t* 转换为 QString
//        QString pQstr = QString::fromWCharArray(pWideStr);
//        qDebug() << "pQstr " << pQstr << '\n';
//        //QString 转换为 std::string
//        std::string stdStr = pQstr.toStdString();
//        qDebug() << "stdStr " << pQstr << '\n';

        ss.clear();
//        ptr = (const char*)value_buff;
        wchar_t * pWideStr = reinterpret_cast<wchar_t*>(value_buff);
        count = 0;
        for(;;)
        {
            len = lstrlen(pWideStr);

            if(len > 0 && count > 0)
            {
                ss << "; ";
            }

            if(len == 0 || count == 128)
            {
                break;
            }
            QString pQstr = QString::fromWCharArray(pWideStr);
            std::string stdStr = pQstr.toStdString();
            ss << stdStr;
            pWideStr += len;
            count++;
        }
        if(len != 0)
        {
            value_string = "Error!";
            return;
        }
        value_string = ss.str();
        qDebug() << QString::fromStdString(value_string) << '\n';
        break;
    }

    default:
    {
        value_string = "";
    }

    }
}


void RegEditTab::addChildKeysData(QString str)
{
    //读取settings childKeys
    QSettings reg(str, QSettings::NativeFormat);
    QStringList keyList = reg.childKeys();

    //key如果带\, 获取注册表的value的时候，qsettings有问题
    //另外REG_SZ，REG_WORD等类型无法知道，改用win api来获取
    //用qt读取key,用windows API解决读取不了带"\"键的键值的问题以及类型

    #define Q_WS_WIN
    #ifdef Q_WS_WIN //声明要使用windows API

    HKEY hKey;
    HKEY hGlobalKey;

    //find subkey string
    std::string keyName; // temp global hkey
//    qDebug() << "str " << str << '\n';

//    const char *subKey = reinterpret_cast<const char*>(str.utf16());
    //QString ==> const char*
    QByteArray inBytes;
    inBytes = str.toUtf8();
    const char *subKey = inBytes.constData();
//    qDebug() << "*subKey" << QString::fromStdString(subKey) << '\n';

/*   Convert QString to Const char *    */

//        QString qtStrData;
//        QByteArray inBytes;
//        const char *cStrData;

//        qtStrData = "String-Data-Here";
//        inBytes = qtStrData.toUtf8();
//        cStrData = inBytes.constData();

//        /*  Convert Const char * to QString */
//        QString qtStrData;
//        qtStrData = QString::fromUtf8(cStrData);

    for(; *subKey != '\0' && *subKey != '\\';
        keyName += *subKey, subKey++); //keyName save global hkey
    // std::string ==> QString
//    qDebug() << "keyName" << QString::fromStdString(keyName) << '\n';

    //iterator over predefined keys
    int i;
    // cast to <int>
    for(i = 0; i != static_cast<int>(key_names_v.size()) &&
                keyName != key_names_v[i]; i++);
//    qDebug() << i << '\n';

    if(i == static_cast<int>(key_names_v.size()))
    {
        qDebug() << "KEY name is wrong!" << '\n';
    }
    else
    {
        hGlobalKey = predef_keys[i];
    }
//    qDebug() << "hGlobalKey" << hGlobalKey << '\n';

    //打开启动项Key
    //const char* ==> LPCWSTR/wchar*
    //LPCWSTR strVariable2 = (const wchar_t*) name.utf16();
//    const wchar_t* w_subKey = reinterpret_cast<const wchar_t*>(subKey);

   ;
    if(strlen(subKey) == 0)
    {
//        qDebug() << "subkey is empty";
         LONG lRet = RegOpenKeyEx(hGlobalKey, L"", 0,
                     KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS|KEY_WOW64_64KEY, &hKey);

        if(lRet != ERROR_SUCCESS)
        {
            //try to open in 32-bit mode
            LONG lRet = RegOpenKeyEx(hGlobalKey, L"", 0,
                                    KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS|KEY_WOW64_32KEY, &hKey);
            if(lRet != ERROR_SUCCESS)
            {
                qDebug() << "failed to read registry key!" << '\n';
            }
        }
    }
    else
    {
        subKey++;
        //convert char* to wchar*
        const WCHAR *pwcsubKey;
        // required size
        int nChars = MultiByteToWideChar(CP_ACP, 0, subKey, -1, NULL, 0);
        // allocate it
        pwcsubKey = new WCHAR[nChars];
        MultiByteToWideChar(CP_ACP, 0, subKey, -1, (LPWSTR)pwcsubKey, nChars);
        //-- convert
        // use it....
        LONG lRet = RegOpenKeyEx(hGlobalKey, pwcsubKey, 0,
                    KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS|KEY_WOW64_64KEY, &hKey);

       if(lRet != ERROR_SUCCESS)
       {
           //try to open in 32-bit mode
           LONG lRet = RegOpenKeyEx(hGlobalKey, pwcsubKey, 0,
                                   KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS|KEY_WOW64_32KEY, &hKey);
           if(lRet != ERROR_SUCCESS)
           {
               qDebug() << "failed to read registry key!" << '\n';
           }
       }

        // delete it
        delete [] pwcsubKey;
        //-- convert
    }

//    qDebug() << "subKey" << QString::fromStdString(subKey) << '\n';
//    qDebug() << "subKey" << reinterpret_cast<const wchar_t*>(subKey) << '\n';
    //遍历key读键值和类型
    #endif

    //遍历并添加到tableWidget
    //清除之前的数据(不包含表头)
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();

    //行变量
    int nRow = 0;
    foreach(QString key,keyList)
    {
        //产生新行
        ui->tableWidget->insertRow(nRow);

//        //QString location = reg.value(key).toString();
//        //键值和类型通过windows API来获取
//        DWORD dwSize = 1024;      //这个值太小，容纳不下返回值的话，RegQueryValueEx会返回一个错误码
        DWORD dwType = REG_DWORD; //值的类型REG_SZ,返回键值类型

        //替换注册表键中的斜线， '/' ==> '\\'
        QString backslash_key = key.replace(QChar('/'), QChar('\\'));

        //char * 转换为 wchar_t * 类型
        const wchar_t * w_key = reinterpret_cast<const wchar_t *>(backslash_key.utf16());

        //只读取到键值字符串的第一个字母，这个是为啥？
        DWORD num_subkeys, max_subkey_len, num_values,
                            max_value_name_len, max_value_len;

        //find max size info regarding the key
        RegQueryInfoKey(hKey, NULL, NULL, NULL,
                    &num_subkeys, &max_subkey_len, NULL, &num_values,
                    &max_value_name_len, &max_value_len, NULL, NULL);
        //value_len + 1
        max_value_len += 1;

        unsigned int bufLen = (unsigned int)max_value_len;

//        WCHAR buff[bufLen]; //表达式的计算结果不是常数
        //C99 variable-sized may not be initialized
//        memset(buff, 0, bufLen);

        LPBYTE buff = nullptr;
//        buff = (LPBYTE)malloc(bufLen);
//        buff = (LPBYTE)malloc(bufLen*sizeof(unsigned char *));
        buff = (LPBYTE)malloc(bufLen*sizeof(LPBYTE));
        //切记初始化，可以解决很多不必要的麻烦
        memset(buff, 0, bufLen*sizeof(LPBYTE));
//        qDebug() << buff << '\n';

        //retrieves the type and data
        DWORD value_len = max_value_len;

        //default value
        DWORD dwRet;
        if(wcscmp(w_key, L".") == 0)
        {
            dwRet  = RegQueryValueEx(hKey, NULL, NULL, &dwType, buff, &value_len);
        }
        else
        {
            dwRet  = RegQueryValueEx(hKey, w_key, NULL, &dwType, buff, &value_len);
        }


        value_len = max_value_len;
//        std::string test= (char*)buff;

        std::string type_string;//register type value
        std::string value_string;//register value data

        if(dwRet != ERROR_SUCCESS)
        {
            qDebug() << "RegQueryValueEx failed " << dwRet;
            RegCloseKey(hKey);
        }
        else
        {
            //Registry Value Types
            parse_value_type(type_string, dwType);

            //Registry Value Data
            parse_value_string(value_string, dwType, buff, value_len);
            //qDebug() << QString::fromStdString(value_string) << '\n';
        }
//        qDebug() << buff << '\n';

        //QString typeName = reg.value(key).typeName();
        //qDebug() << typeName << endl;
        //名称
        //. ==> (默认)
        if(key.compare(".") == 0)
        {
            key.replace(".", QStringLiteral("(默认)"));
        }
        QTableWidgetItem* item1 = new QTableWidgetItem(key);
        ui->tableWidget->setItem(nRow, 0, item1);


        //类型
        //std::string 转换为 QString
        QString regType = QString::fromStdString(type_string);
        QTableWidgetItem* item2 = new QTableWidgetItem(regType);
        ui->tableWidget->setItem(nRow, 1, item2);

        //数据
        QString regValue = QString::fromStdString(value_string);
        QTableWidgetItem* item3 = new QTableWidgetItem(regValue);
        ui->tableWidget->setItem(nRow, 2, item3);

        nRow++;

        free(buff);
    }
    RegCloseKey(hKey);
}

void RegEditTab::OnTreeViewDoubleClick(const QModelIndex &index)
{
    //    QAbstractItemModel* m = (QAbstractItemModel*)index.model();
    //    qDebug() << index.row() <<  m->columnCount() << endl;
    //    for(int columnIndex = 0; columnIndex < m->columnCount(); columnIndex++)
    //    {
    //        QModelIndex x = m->itemFromIndex(index);

    //        QString s = x.data().toString();
    //        //QMessageBox::about(this,s,s);
    //        qDebug() << s <<endl;
    //    }

    //选中的行
    QModelIndex selectedIndex = ui->treeView->currentIndex();

    //获取选中行数据
    QString strPath = selectedIndex.data().toString();
    //qDebug() << strPath << endl;

    //当前节点
    QStandardItem* currentItem = m_StdItemModel->itemFromIndex(selectedIndex);
    //获取绝对路径
    QString strAbsolutePath = getAbsolutePath(selectedIndex);
    //    qDebug() << "getAbsolutePath" << strAbsolutePath;
    //设置lineEdit的text
    ui->lineEdit->setText(strAbsolutePath);

    QString strAbsPathTrue = replaceHCC(strAbsolutePath);
    //qDebug() << strAbsPathTrue << endl;

    //节点为空
    if(currentItem->rowCount() == 0)
    {
        //遍历并添加childGroups
        addChildGroups(strAbsPathTrue, currentItem);
    }
}

//void RegEditTab::test()
//{
//   int softWareCount=0;
//   softWareList->setFrameShape(QFrame::NoFrame);
//   QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",QSettings::NativeFormat);
//   QStringList groupsList=reg.childGroups();
//   foreach(QString group,groupsList)
//   {
//       reg.beginGroup(group);
//       QString softWareName=reg.value("DisplayName").toString();
//       if(!softWareName.isEmpty())
//       {
//           softWareList->addItem(new QListWidgetItem(softWareName));
//           softWareCount++;
//       }
//       reg.endGroup();
//   }

//---------------------------------------------------
//    int softWareCount=0;
//    ui->listView->setFrameShape(QFrame::NoFrame);
//    QSettings reg("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",QSettings::NativeFormat);
//    QSettings reg("HKEY_CLASSES_ROOT",QSettings::NativeFormat);
//    QSettings reg("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current",QSettings::NativeFormat);
//    QStringList groupsList=reg.childGroups();

//    QStringListModel* slm = new QStringListModel(this);
//    QStringList* sl = new QStringList();


//    foreach(QString group,groupsList)
//    {
//        reg.beginGroup(group);
//        //QString softWareName=reg.value("DisplayName").toString();
////        if(!softWareName.isEmpty())
////        {
////            sl->append(softWareName);
////            softWareCount++;
////        }
//        reg.endGroup();
//    }
////    slm->setStringList(*sl);
//    slm->setStringList(groupsList);
//    ui->listView->setModel(slm);
//}

QString RegEditTab::getAbsolutePath(QModelIndex itemIndex)
{
    QString strIndex = itemIndex.data().toString();
    //    QString strAbsIndex;
    QModelIndex secondItem = itemIndex;

    while(itemIndex.parent().isValid())
    {
        secondItem = itemIndex.parent();
        //拼接字符串
        strIndex.insert(0, secondItem.data().toString().append("\\"));
        itemIndex = secondItem;
    }

    //移除根目录
    if(strIndex.indexOf(QStringLiteral("我的电脑\\")) != -1)
    {
        strIndex.remove(QStringLiteral("我的电脑\\"));
    }

    return strIndex;
}

QString RegEditTab::replaceHCC(QString str)
{
    //"HKEY_CURRENT_CONFIG" 需要替换
    //HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current
    //QStandardItem* currentItem = model->itemFromIndex(currentIndex);
    QString strTrueCurrentPath = "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current";
    //    if(str.compare(QString::fromLocal8Bit("HKEY_CURRENT_CONFIG")) == 0)
    //    {
    //        str.replace(str, strTrueCurrentPath);
    //    }
    //qDebug() << strPath << endl;
    if(str.indexOf("HKEY_CURRENT_CONFIG") != -1)
    {
        str.replace("HKEY_CURRENT_CONFIG",
                    "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Hardware Profiles\\Current");
    }

    return str;
}

void RegEditTab::createMenu()
{
    //novalid menu
    m_pTableWidgetMenu = new QMenu(ui->tableWidget);
    m_pActNewMenu = m_pTableWidgetMenu->addMenu(QString::fromLocal8Bit("新建(N)"));

    //valid menu
    m_pTableWgtMenuValid = new QMenu(ui->tableWidget);
}

void RegEditTab::createAction()
{
    //novalid menu
    m_pActStringValue = new QAction(ui->tableWidget);
    m_pActStringValue->setText(QString::fromLocal8Bit("字符串值(S)"));

    m_pActNewMenu->addAction(m_pActStringValue);

    //valid menu
    m_pActModify = new QAction(QString::fromLocal8Bit("修改(M)"), ui->tableWidget);
    m_pTableWgtMenuValid->addAction(m_pActModify);
    m_pActRename = new QAction(QString::fromLocal8Bit("重命名(R)"), ui->tableWidget);
    m_pTableWgtMenuValid->addAction(m_pActRename);
    m_pActDelete = new QAction(QString::fromLocal8Bit("删除(M)"), ui->tableWidget);
    m_pTableWgtMenuValid->addAction(m_pActDelete);
}

void RegEditTab::createContextMenu()
{
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

void RegEditTab::setMenuStatus()
{
    //get lineEdit text
    QString strLine = ui->lineEdit->text();
//    qDebug() << strLine;

    if(strLine.isEmpty() || strLine.compare(QString::fromLocal8Bit("我的电脑")) == 0)
    {
        //create string value is disable
        m_pActStringValue->setEnabled(false);
    }
    else
    {
        //enable
        m_pActStringValue->setEnabled(true);
    }
}

void RegEditTab::createDefaultRegStringValue()
{
//    //get current row count
//    int nRow = ui->tableWidget->rowCount();
//    qDebug() << "1=>" << nRow << '\n';
//    //new row
//    ++nRow;
//    qDebug() << "2=>" << nRow << '\n';
//    ui->tableWidget->insertRow(nRow);
//    //insert data
//    QTableWidgetItem *item1 = new QTableWidgetItem(QString::fromLocal8Bit("新值 #"));
//    ui->tableWidget->setItem(nRow, 0, item1);

//    QTableWidgetItem *item2 = new QTableWidgetItem(QString::fromLocal8Bit("REG_SZ"));
//    ui->tableWidget->setItem(nRow, 1, item2);
//    qDebug() << "createDefaultRegStringValue" << '\n';

    //get lineEdit text
    QString strPath = ui->lineEdit->text();

    //QSettings
    QSettings settings(strPath, QSettings::NativeFormat);
    settings.setValue(QString::fromLocal8Bit("新值 #"), "");

    //ui tableWidget update
    OnTreeViewClick(QModelIndex());
}

void RegEditTab::setRegNewStringValue(QString str)
{
    //get lineEdit text
    QString strPath = ui->lineEdit->text();

    //QSettings
    QSettings settings(strPath, QSettings::NativeFormat);
    settings.setValue(QString::fromLocal8Bit("新值 #"), str);

    //ui tableWidget update
    OnTreeViewClick(QModelIndex());
}

void RegEditTab::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "keyPressEvent(QKeyEvent *event)" << '\n';
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        qDebug() << "enter" << '\n';
    }
}

bool RegEditTab::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        qDebug("Ate key press %d", keyEvent->key());
        //.... do something
        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
        {
           setRegNewStringValueKey();
        }

        return true;
    }
    else if(event->type() == QEvent::MouseButtonDblClick)
    {
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        QMouseEvent *keyEvent = static_cast<QMouseEvent *>(event);
        keyEvent->Show;
//        qDebug("Ate key press %d", keyEvent->key());
    }
//    else if (obj == ui->tableWidget->viewport())
//    {

//        QEvent::MouseButtonDblClick
//        if (event->type() == QEvent::MouseButtonPress){
//            emit sig_focusChanged();
//        }
//    }
    else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

void RegEditTab::OnTableWCustomContextMenuRequested(const QPoint &pos)
{
    //item true or false
//    QTableWidgetItem *item = ui->tableWidget->itemAt(pos);
    QModelIndex index = ui->tableWidget->indexAt(pos);
    qDebug() << index << '\n';

    if(index.isValid())
    {
//        qDebug() << "isValid" << '\n';
        m_pTableWgtMenuValid->exec(QCursor::pos());
    }
    else //not empty
    {
//        qDebug() << "noValid" << '\n';
        //set menu status
        setMenuStatus();

        //mouse in any where show
        m_pTableWidgetMenu->exec(QCursor::pos());
    }
//    qDebug() << "OnTableWidgetCustomContextMenuRequested" << '\n';
}

void RegEditTab::createRegStringValue()
{
//    qDebug() << "createRegStringValue()" << '\n';
//    //未选择注册表路径，新建字符串值菜单不可用
//    QString strLine = ui->lineEdit->text();
//    qDebug() << strLine;

    createDefaultRegStringValue();
}

void RegEditTab::modifyRegSlot()
{
//    qDebug() << "modifyRegSlot()" << '\n';
//    QTableWidgetItem *item = ui->tableWidget->currentItem();

    //get index
    QModelIndex index = ui->tableWidget->currentIndex();
    //test index row
    qDebug() << index.row() << '\n';

    //get current row, column 0 data
    QString strKey = ui->tableWidget->item(index.row(), 0)->text();
    QString strVal = ui->tableWidget->item(index.row(), 2)->text();

    regeditMod->setData(strKey, strVal);

    //accept()
    regeditMod->exec();
}

void RegEditTab::setRegNewStringValueKey()
{
    //can be modified
    editFlag = true;

    //save regedit old key
    QModelIndex index = ui->tableWidget->currentIndex();
    strOldKey = ui->tableWidget->item(index.row(), 0)->text();
}

void RegEditTab::renameRegSlot()
{
    qDebug() << "renameRegSlot" << '\n';
    //get currentIndex
    //get index
    QModelIndex index = ui->tableWidget->currentIndex();
    //test index row
    qDebug() << index.row() << '\n';

    //set column 0 can be edit
    //ui->tableWidget->item(index.row(),0)->setFlags(Qt::ItemIsEditable);

    //select index row, column 0
    //ui->tableWidget->setCurrentCell(index.row(), 0, QItemSelectionModel::Select);

    //
    //ui->tableWidget->setEditTriggers(QAbstractItemView::SelectedClicked);

    //ui->tableWidget->editItem(ui->tableWidget->item(index.row(), 0));

    ui->tableWidget->setFocus();
//    ui->tableWidget->item(index.row(), 0)->setFlags(ui->tableWidget->item(index.row(), 0)->flags() &
//                                                    Qt::ItemIsEditable);

//    ui->tableWidget->item(index.row(), 0)->setFlags(ui->tableWidget->item(index.row(), 0)->flags() &
//                                                    Qt::ItemIsSelectable);
    ui->tableWidget->editItem(ui->tableWidget->item(index.row(), 0));

//    ui->tableWidget->item(index.row(), 0)->setFlags((Qt::ItemFlags) 63);

}

void RegEditTab::deleteRegSlot()
{
    QModelIndex index = ui->tableWidget->currentIndex();
    QString strKey = ui->tableWidget->item(index.row(), 0)->text();

    if(strKey.isEmpty())
    {
        return;
    }

    //QSettings
    //get lineEdit text
    QString strPath = ui->lineEdit->text();

    //delete regedit key-value
    QSettings settings(strPath, QSettings::NativeFormat);

    settings.remove(strKey);

    //tableWidget ui update
    OnTreeViewClick(QModelIndex());
}

void RegEditTab::receiveData(QString str)
{
    //setting regedit value
    setRegNewStringValue(str);
}

void RegEditTab::receiveModData(QString str)
{
    QModelIndex index = ui->tableWidget->currentIndex();

    //key
    QTableWidgetItem *item = ui->tableWidget->item(index.row(), 0);
    QString key = item->text();

    if(key.isEmpty())
    {
        return;
    }

    qDebug() << "key " << key << '\n';
    //old value
//    QTableWidgetItem *item2 = ui->tableWidget->item(index.row(), 2);
//    QString val = item2->text();
//    qDebug() << "val " << val << '\n';

    //QSettings
    //QSettings
    //get lineEdit text
    QString strPath = ui->lineEdit->text();
    qDebug() << "strPath " << strPath << '\n';
    //add regedit key-value
    QSettings settings(strPath, QSettings::NativeFormat);
    settings.setValue(key, str);

    //ui tableWidget update
    OnTreeViewClick(QModelIndex());
}

void RegEditTab::itemChangedSlot(QTableWidgetItem *item)
{
    if(editFlag)
    {
//        qDebug() << "itemChangedSlot" << item->data(Qt::DisplayRole).toString();

        //get current edit item
        //get index
        QModelIndex index = ui->tableWidget->currentIndex();
//        QString strNewKey = ui->tableWidget->item(index.row(), 0)->text();
//        qDebug() << "strNewKey " << strNewKey << '\n'; //同下
        QString strNewKey = item->data(Qt::DisplayRole).toString();

        qDebug() << "strNewKey " << strNewKey << '\n';
        QString strVal = ui->tableWidget->item(index.row(), 2)->text();
        qDebug() <<"strVal " << strVal << '\n';

        //QSettings
        //get lineEdit text
        QString strPath = ui->lineEdit->text();
        qDebug() << "strPath " << strPath << '\n';
        //add regedit key-value
        QSettings settings(strPath, QSettings::NativeFormat);
        settings.setValue(strNewKey, strVal);

        if(!strOldKey.isEmpty())
        {
            //delete old regedit key-value
            settings.remove(strOldKey);
            //recovery regedit old key false string to empty
            strOldKey = "";
        }

        //recovery edit flag
        editFlag = false;

        //ui tableWidget update
        OnTreeViewClick(QModelIndex());
    }
}


void RegEditTab::itemEnteredSlot(QTableWidgetItem *item)
{
    qDebug() << "itemEnteredSlot" << item->data(Qt::DisplayRole).toString();
}

void RegEditTab::cellDoubleClickedSlot(int row, int column)
{
    qDebug() << "cellDoubleClickedSlot(int row, int column)" << '\n';
    //get regedit key
    QTableWidgetItem *item = ui->tableWidget->item(row, 0);
    QString key = item->text();

    //get regedit value
    QTableWidgetItem *item2 = ui->tableWidget->item(row, 2);
    QString val = item2->text();

    //send key-value
    regeditMod->setData(key, val);

    //exec

    regeditMod->exec();
}

void RegEditTab::currentCellChangedSlot(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    qDebug() << "currentCellChangedSlot";
}

void RegEditTab::OnTreeViewClick(const QModelIndex &index)
{
    qDebug() << "OnTreeViewClick(const QModelIndex &index)" << endl;
    //遍历并添加 childKeys
    addChildKeys();
}
