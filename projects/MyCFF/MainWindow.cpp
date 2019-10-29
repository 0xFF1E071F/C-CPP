#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
#include <iomanip>
#include <string>

#include <QTableWidget>
#include <QDebug>

using namespace std;

//rc
#define TV_FILE_ROOT  QStringLiteral("File:")

#define TV_DOS_HEADER QStringLiteral("Dos Header")
#define TV_NT_HEADERS  QStringLiteral("Nt Headers")
#define TV_SECTION_HEADERS QStringLiteral("SectionHeaders [x]")
#define TV_EXPORT_DIRECTORY QStringLiteral("Export Directory")
#define TV_IMPORT_DIRECTORY QStringLiteral("Import Directory")
#define TV_TLS_DIRECTORY QStringLiteral("TLS Directory")
#define TV_RELOCATION_DIRECTORY QStringLiteral("Relocation Directory")
#define TV_ADDRESS_CONVERTER QStringLiteral("Address Converter")

#define TV_NT_FH QStringLiteral("File Header")
#define TV_NT_OH QStringLiteral("Optional Header")

#define TV_NT_OH_DD QStringLiteral("Data Directories [x]")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //属性初始化
    MyInitialize();

    //设置信号槽
    SetConnect();
    //设置UI属性
    SetUI();

    //初始化左侧树目录
    InitTreeDir();
}

MainWindow::~MainWindow()
{
    delete ui;

    if (m_hFileRead != nullptr)
    {
        m_hFileRead = nullptr;
    }
    if (m_hMapFile != nullptr)
    {
        m_hMapFile = nullptr;
    }
    if (m_hImageBase != nullptr)
    {
        m_hImageBase = nullptr;
    }
    if (m_pDH != nullptr)
    {
        m_pDH = nullptr;
    }
    if (m_pNTH != nullptr)
    {
        m_pNTH = nullptr;
    }
    if (m_pFH != nullptr)
    {
        m_pFH = nullptr;
    }
    if (m_pOH32 != nullptr)
    {
        m_pOH32 = nullptr;
    }
    if (m_pSH != nullptr)
    {
        m_pSH = nullptr;
    }
    
}

void MainWindow::MyInitialize()
{
    m_hFileRead = nullptr;
    m_hMapFile = nullptr;
    m_hImageBase = nullptr;
    m_pDH = nullptr;
    m_pNTH = nullptr;
    m_pFH = nullptr;
    m_pOH32 = nullptr;
    m_pSH = nullptr;
}

void MainWindow::SetConnect()
{
    //Treeview 单击事件
    QObject::connect(ui->treeView,
        SIGNAL(clicked(const QModelIndex)),
        this,
        SLOT(OnTreeViewClickSlot(const QModelIndex)));

    //TableWidget clicked事件
    QObject::connect(ui->tw_pesh_1,
        SIGNAL(clicked(const QModelIndex)),
        this,
        SLOT(AddSectionOffsetSlot(const QModelIndex)));

    //导出表
    QObject::connect(ui->tw_export_dir_2,
        SIGNAL(clicked(const QModelIndex)),
        this,
        SLOT(AddExportFuncOffsetSlot(const QModelIndex)));

    //导入表 tw_peid_1 clicked事件，显示偏移和导入函数
    QObject::connect(ui->tw_peid_1,
        SIGNAL(clicked(const QModelIndex)),
        this,
        SLOT(ShowImpDirOffsetAndFuncSlot(const QModelIndex)));

    QObject::connect(ui->tw_peid_2,
        SIGNAL(clicked(const QModelIndex)),
        this,
        SLOT(ShowImpDirFuncOffsetSlot(const QModelIndex)));
    //重定位表
    QObject::connect(ui->tw_perd_1,
        SIGNAL(clicked(const QModelIndex)),
        this,
        SLOT(AddRelocationOffsetSlot(const QModelIndex)));
    QObject::connect(ui->tw_perd_2,
        SIGNAL(clicked(const QModelIndex)),
        this,
        SLOT(AddRelocItemOffsetSlot(const QModelIndex)));

    //地址转换
    QObject::connect(ui->lineEdit_VA, 
        SIGNAL(returnPressed()), 
        this, 
        SLOT(VA2RVA2FASlot()));

    QObject::connect(ui->lineEdit_RVA,
        SIGNAL(returnPressed()),
        this,
        SLOT(RVA2FA2VASlot()));

    QObject::connect(ui->lineEdit_FA,
        SIGNAL(returnPressed()),
        this,
        SLOT(FA2VA2RVASlot()));
}

void MainWindow::SetUI()
{
    //设置窗口标题
    setWindowTitle("MyPE");

    //TreeView
    //设置选择行为，以行为单位
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    //ui->treeView->setFocusPolicy(Qt::NoFocus);
    //隐藏默认表头
    ui->treeView->header()->hide();
    //设置treewidget水平滚动条
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->treeView->header()->setStretchLastSection(false);

    //TableWidget
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

    //tw_pesh_1
    //设置选择行为，以行为单位
    ui->tw_pesh_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tw_pesh_1->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->tw_pesh_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->tw_pesh_1->setFocusPolicy(Qt::NoFocus);

    //自适应标题头
    ui->tw_pesh_1->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tw_pesh_1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //tw_pesh_1
    //设置选择行为，以行为单位
    ui->tw_pesh_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tw_pesh_2->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->tw_pesh_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->tw_pesh_2->setFocusPolicy(Qt::NoFocus);

    //自适应标题头
    ui->tw_pesh_2->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tw_pesh_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //tw_peid_1
    //设置选择行为，以行为单位
    ui->tw_peid_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tw_peid_1->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->tw_peid_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->tw_peid_1->setFocusPolicy(Qt::NoFocus);

    //自适应标题头
    ui->tw_peid_1->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tw_peid_1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //tw_peid_2
    //设置选择行为，以行为单位
    ui->tw_peid_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tw_peid_2->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->tw_peid_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->tw_peid_2->setFocusPolicy(Qt::NoFocus);

    //自适应标题头
    ui->tw_peid_2->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tw_peid_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //重定位
    //tw_perd_1
    //设置选择行为，以行为单位
    ui->tw_perd_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tw_perd_1->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->tw_perd_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->tw_perd_1->setFocusPolicy(Qt::NoFocus);

    //自适应标题头
    ui->tw_perd_1->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tw_perd_1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //tw_perd_2
    //设置选择行为，以行为单位
    ui->tw_perd_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tw_perd_2->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->tw_perd_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    ui->tw_perd_2->setFocusPolicy(Qt::NoFocus);
    //自适应标题头
    ui->tw_perd_2->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tw_perd_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //开启拖拽
    this->setAcceptDrops(true);
}

void MainWindow::InitTreeDir()
{
    m_StdItemModel = new QStandardItemModel(ui->treeView);

    QStandardItem *RootItem = new QStandardItem(QStringLiteral("File:"));
    m_StdItemModel->appendRow(RootItem);

    //QStandardItem *item4 = new QStandardItem(QStringLiteral("HKEY_USERS"));
    //QStandardItem *item5 = new QStandardItem(QStringLiteral("HKEY_CURRENT_CONFIG"));

    //一级节点
    QStandardItem *itemDh = new QStandardItem(QStringLiteral("Dos Header"));
    QStandardItem *itemNt = new QStandardItem(QStringLiteral("Nt Headers"));
    QStandardItem *itemSh = new QStandardItem(QStringLiteral("SectionHeaders [x]"));
    QStandardItem *itemED = new QStandardItem(TV_EXPORT_DIRECTORY);
    QStandardItem *itemID = new QStandardItem(QStringLiteral("Import Directory"));
    QStandardItem* itemTD = new QStandardItem(TV_TLS_DIRECTORY);
    QStandardItem *itemRD = new QStandardItem(TV_RELOCATION_DIRECTORY);
    QStandardItem *itemAC = new QStandardItem(QStringLiteral("Address Converter"));
    RootItem->appendRow(itemDh);  //Dos Header
    RootItem->appendRow(itemNt);  //Nt Headers
    RootItem->appendRow(itemSh);  //SectionHeaders [x]
    RootItem->appendRow(itemED);
    RootItem->appendRow(itemID); //Import Directory
    RootItem->appendRow(itemTD);
    RootItem->appendRow(itemRD);
    RootItem->appendRow(itemAC); //Address Converter
    //RootItem->appendRow(item4);
    //RootItem->appendRow(item5);

    //二级节点
    QStandardItem *itemNt_Fh = new QStandardItem(QStringLiteral("File Header"));
    QStandardItem *itemNt_Oh = new QStandardItem(QStringLiteral("Optional Header"));
    itemNt->appendRow(itemNt_Fh);  //File Header
    itemNt->appendRow(itemNt_Oh);  //Optional Header

    //三级节点
    QStandardItem *itemNt_Oh_Dd = new QStandardItem(QStringLiteral("Data Directories [x]"));
    itemNt_Oh->appendRow(itemNt_Oh_Dd);  //Data Directories [x]

    ui->treeView->setModel(m_StdItemModel);

    //展开根节点
    //QModelIndex indexRoot = m_StdItemModel->index(0, 0, QModelIndex());
    //ui->treeView->selectionModel();
    //ui->treeView->expand(indexRoot);
    ui->treeView->expandAll();
}

void MainWindow::GetErrorMsg()
{
    LPWSTR pBuffer = NULL;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPWSTR)&pBuffer,
                  0,
                  NULL);

    MessageBox(NULL, pBuffer, 0, 0);

    //Free the buffer
    LocalFree(pBuffer);
}

void MainWindow::GetDataType(int n, char *pDataType)
{
    switch (n)
    {
        case 1:
            strcpy(pDataType, "BYTE");
            break;
        case 2:
            strcpy(pDataType, "WORD");
            break;
        case 4:
            strcpy(pDataType, "DWORD");
            break;
        default:
            strcpy(pDataType, "");
            break;
    }
}

void MainWindow::GetDataTypeAndVal(IN const char * szPEHeaderType, //PE头文件类型
                                   IN void * p,              //PE指针
                                   IN char * szBuf,          //字段名
                                   OUT char * pszDataType,   //数据类型，如BYTE, WORD, DWORD,
                                   OUT void * pVal)          //字段值，到外面需要再次转换
{
    const char *pAry_PE_Header[] = {"IMAGE_DOS_HEADER", 
                                    "IMAGE_NT_HEADERS", 
                                    "IMAGE_FILE_HEADER", 
                                    "IMAGE_OPTIONAL_HEADER32", 
                                    "IMAGE_OPTIONAL_HEADER64",
                                    "IMAGE_DATA_DIRECTORY",
                                    "IMAGE_SECTION_HEADER"};
    int n = 0;
    for (size_t i=0; i<sizeof(pAry_PE_Header)/sizeof(pAry_PE_Header[0]); i++)
    {
        if (strcmp(szPEHeaderType, pAry_PE_Header[i]) == 0)
        {
            n = i + 1;
            break;
        }
    }

    PIMAGE_OPTIONAL_HEADER32 pOH = nullptr;

    switch (n)
    {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            pOH = (PIMAGE_OPTIONAL_HEADER32)p;
            if (strcmp(szBuf, "Magic") == 0)
            {
                GetDataType(sizeof(pOH->Magic), pszDataType);
                pVal = (void*)pOH->Magic;
            }
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        default:
            break;
    }
    
}

void MainWindow::GetDataTypeAndVal(IN const char * szPEHeaderType,
    IN void * p, 
    IN char * szBuf, 
    OUT char * pszDataType, 
    OUT DWORD &dwVal)
{
    const char *pAry_PE_Header[] = { "IMAGE_DOS_HEADER",        //0    -->  1
                                    "IMAGE_NT_HEADERS",         //1    -->  2
                                    "IMAGE_FILE_HEADER",        //2
                                    "IMAGE_OPTIONAL_HEADER32",  //3
                                    "IMAGE_OPTIONAL_HEADER64",  //4
                                    "IMAGE_DATA_DIRECTORY",     //5
                                    "IMAGE_SECTION_HEADER" ,    //6
                                    "IMAGE_EXPORT_DIRECTORY",   //7
                                    "IMAGE_TLS_DIRECTORY"       //8
                                    };  
    PIMAGE_DOS_HEADER pDH = nullptr;
    PIMAGE_NT_HEADERS pNH = nullptr;
    PIMAGE_FILE_HEADER pFH = nullptr;
    PIMAGE_OPTIONAL_HEADER32 pOH = nullptr;
    PIMAGE_OPTIONAL_HEADER64 pOH64 = nullptr;
    PIMAGE_DATA_DIRECTORY pDD = nullptr;
    PIMAGE_SECTION_HEADER pSH = nullptr;
    PIMAGE_EXPORT_DIRECTORY pExportDir = nullptr;
    PIMAGE_TLS_DIRECTORY pTlsDir = nullptr;

    int n = 0;

    for (size_t i = 0; i < sizeof(pAry_PE_Header) / sizeof(pAry_PE_Header[0]); i++)
    {
        if (strcmp(szPEHeaderType, pAry_PE_Header[i]) == 0)
        {
            n = i + 1;
            break;
        }
    }


    switch (n)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        pOH = (PIMAGE_OPTIONAL_HEADER32)p;
        if (strcmp(szBuf, "Magic") == 0)
        {
            GetDataType(sizeof(pOH->Magic), pszDataType);
            dwVal = pOH->Magic;
        }
        if (strcmp(szBuf, "MajorLinkerVersion") == 0)
        {
            GetDataType(sizeof(pOH->MajorLinkerVersion), pszDataType);
            dwVal = pOH->MajorLinkerVersion;
        }
        if (strcmp(szBuf, "MinorLinkerVersion") == 0)
        {
            GetDataType(sizeof(pOH->MajorLinkerVersion), pszDataType);
            dwVal = pOH->MajorLinkerVersion;
        }
        if (strcmp(szBuf, "SizeOfCode") == 0)
        {
            GetDataType(sizeof(pOH->SizeOfCode), pszDataType);
            dwVal = pOH->SizeOfCode;
        }
        if (strcmp(szBuf, "SizeOfInitializedData") == 0)
        {
            GetDataType(sizeof(pOH->SizeOfInitializedData), pszDataType);
            dwVal = pOH->SizeOfInitializedData;
        }
        if (strcmp(szBuf, "SizeOfUninitializedData") == 0)
        {
            GetDataType(sizeof(pOH->SizeOfUninitializedData), pszDataType);
            dwVal = pOH->SizeOfUninitializedData;
        }
        if (strcmp(szBuf, "AddressOfEntryPoint") == 0)
        {
            GetDataType(sizeof(pOH->AddressOfEntryPoint), pszDataType);
            dwVal = pOH->AddressOfEntryPoint;
        }
        if (strcmp(szBuf, "BaseOfCode") == 0)
        {
            GetDataType(sizeof(pOH->BaseOfCode), pszDataType);
            dwVal = pOH->BaseOfCode;
        }
        if (strcmp(szBuf, "BaseOfData") == 0)
        {
            GetDataType(sizeof(pOH->BaseOfData), pszDataType);
            dwVal = pOH->BaseOfData;
        }
        if (strcmp(szBuf, "ImageBase") == 0)
        {
            GetDataType(sizeof(pOH->ImageBase), pszDataType);
            dwVal = pOH->ImageBase;
        }
        if (strcmp(szBuf, "SectionAlignment") == 0)
        {
            GetDataType(sizeof(pOH->SectionAlignment), pszDataType);
            dwVal = pOH->SectionAlignment;
        }
        if (strcmp(szBuf, "FileAlignment") == 0)
        {
            GetDataType(sizeof(pOH->FileAlignment), pszDataType);
            dwVal = pOH->FileAlignment;
        }
        if (strcmp(szBuf, "MajorOperatingSystemVersion") == 0)
        {
            GetDataType(sizeof(pOH->MajorOperatingSystemVersion), pszDataType);
            dwVal = pOH->MajorOperatingSystemVersion;
        }
        if (strcmp(szBuf, "MinorOperatingSystemVersion") == 0)
        {
            GetDataType(sizeof(pOH->MinorOperatingSystemVersion), pszDataType);
            dwVal = pOH->MinorOperatingSystemVersion;
        }
        if (strcmp(szBuf, "MajorImageVersion") == 0)
        {
            GetDataType(sizeof(pOH->MajorImageVersion), pszDataType);
            dwVal = pOH->MajorImageVersion;
        }
        if (strcmp(szBuf, "MinorImageVersion") == 0)
        {
            GetDataType(sizeof(pOH->MinorImageVersion), pszDataType);
            dwVal = pOH->MinorImageVersion;
        }
        if (strcmp(szBuf, "MajorSubsystemVersion") == 0)
        {
            GetDataType(sizeof(pOH->MajorSubsystemVersion), pszDataType);
            dwVal = pOH->MajorSubsystemVersion;
        }
        if (strcmp(szBuf, "MinorSubsystemVersion") == 0)
        {
            GetDataType(sizeof(pOH->MinorSubsystemVersion), pszDataType);
            dwVal = pOH->MinorSubsystemVersion;
        }
        if (strcmp(szBuf, "Win32VersionValue") == 0)
        {
            GetDataType(sizeof(pOH->Win32VersionValue), pszDataType);
            dwVal = pOH->Win32VersionValue;
        }
        if (strcmp(szBuf, "SizeOfImage") == 0)
        {
            GetDataType(sizeof(pOH->SizeOfImage), pszDataType);
            dwVal = pOH->SizeOfImage;
        }
        if (strcmp(szBuf, "SizeOfHeaders") == 0)
        {
            GetDataType(sizeof(pOH->SizeOfHeaders), pszDataType);
            dwVal = pOH->SizeOfHeaders;
        }
        if (strcmp(szBuf, "CheckSum") == 0)
        {
            GetDataType(sizeof(pOH->CheckSum), pszDataType);
            dwVal = pOH->CheckSum;
        }
        if (strcmp(szBuf, "Subsystem") == 0)
        {
            GetDataType(sizeof(pOH->Subsystem), pszDataType);
            dwVal = pOH->Subsystem;
        }
        if (strcmp(szBuf, "DllCharacteristics") == 0)
        {
            GetDataType(sizeof(pOH->DllCharacteristics), pszDataType);
            dwVal = pOH->DllCharacteristics;
        }
        if (strcmp(szBuf, "SizeOfStackReserve") == 0)
        {
            GetDataType(sizeof(pOH->SizeOfStackReserve), pszDataType);
            dwVal = pOH->SizeOfStackReserve;
        }
        if (strcmp(szBuf, "SizeOfStackCommit") == 0)
        {
            GetDataType(sizeof(pOH->SizeOfStackCommit), pszDataType);
            dwVal = pOH->SizeOfStackCommit;
        }
        if (strcmp(szBuf, "SizeOfHeapReserve") == 0)
        {
            GetDataType(sizeof(pOH->SizeOfHeapReserve), pszDataType);
            dwVal = pOH->SizeOfHeapReserve;
        }
        if (strcmp(szBuf, "SizeOfHeapCommit") == 0)
        {
            GetDataType(sizeof(pOH->SizeOfHeapCommit), pszDataType);
            dwVal = pOH->SizeOfHeapCommit;
        }
        if (strcmp(szBuf, "LoaderFlags") == 0)
        {
            GetDataType(sizeof(pOH->LoaderFlags), pszDataType);
            dwVal = pOH->LoaderFlags;
        }
        if (strcmp(szBuf, "NumberOfRvaAndSizes") == 0)
        {
            GetDataType(sizeof(pOH->NumberOfRvaAndSizes), pszDataType);
            dwVal = pOH->NumberOfRvaAndSizes;
        }
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        FormatSectionRetVal(pSH, szBuf, dwVal);
        break;
    case 8:
        pExportDir = (PIMAGE_EXPORT_DIRECTORY)p;
        FormatExportDirMember(pExportDir, szBuf, dwVal, pszDataType);
        break;
    case 9:
        pTlsDir = (PIMAGE_TLS_DIRECTORY)p;
        FormatTlsDirMember(pTlsDir, szBuf, dwVal, pszDataType);
    default:
        break;
    }
}

void MainWindow::GetMeaning(WORD n, char * pszMeaning)
{
    switch (n)
    {
        case IMAGE_FILE_MACHINE_UNKNOWN:
            strcpy(pszMeaning, "Unknown value");
            break;

        case IMAGE_FILE_MACHINE_TARGET_HOST:
            strcpy(pszMeaning, "Target Host");
            break;

        case IMAGE_FILE_MACHINE_I386:
            strcpy(pszMeaning, "Intel 386");
            break;

        case IMAGE_FILE_MACHINE_R3000:
            strcpy(pszMeaning, "R3000 - MIPS"); 
            break;
 
        case IMAGE_FILE_MACHINE_R4000:
            strcpy(pszMeaning, "R4000 - MIPS");
            break;

        case IMAGE_FILE_MACHINE_R10000:
            strcpy(pszMeaning, "R10000 - MIPS");
            break;

        case IMAGE_FILE_MACHINE_WCEMIPSV2:
            strcpy(pszMeaning, "MIPS WCE v2");
            break;

        case IMAGE_FILE_MACHINE_ALPHA:
            strcpy(pszMeaning, "Alpha_AXP");
            break;

        case IMAGE_FILE_MACHINE_SH3:
            strcpy(pszMeaning, "SH3");
            break;

        case IMAGE_FILE_MACHINE_SH3DSP:
            strcpy(pszMeaning, "SH3DSP");
            break;

        case IMAGE_FILE_MACHINE_SH3E:
            strcpy(pszMeaning, "SH3E");
            break;

        case IMAGE_FILE_MACHINE_SH4:
            strcpy(pszMeaning, "SH4");
            break;

        case IMAGE_FILE_MACHINE_SH5:
            strcpy(pszMeaning, "SH5");
            break;

        case IMAGE_FILE_MACHINE_ARM:
            strcpy(pszMeaning, "ARM");
            break;

        case IMAGE_FILE_MACHINE_THUMB:
            strcpy(pszMeaning, "ARM Thumb");
            break;

        case IMAGE_FILE_MACHINE_ARMNT:
            strcpy(pszMeaning, "ARM Thumb-2");
            break;

        case IMAGE_FILE_MACHINE_AM33:
            strcpy(pszMeaning, "ARM AM33");
            break;

        case IMAGE_FILE_MACHINE_POWERPC:
            strcpy(pszMeaning, "IBM PowerPC");
            break;

        case IMAGE_FILE_MACHINE_POWERPCFP:
            strcpy(pszMeaning, "IBM PowerPC FP");
            break;

        case IMAGE_FILE_MACHINE_IA64:
            strcpy(pszMeaning, "Intel 64");
            break;

        case IMAGE_FILE_MACHINE_MIPS16:
            strcpy(pszMeaning, "MIPS16");
            break;

        case IMAGE_FILE_MACHINE_MIPSFPU:
            strcpy(pszMeaning, "MIPSFPU");
            break;

        case IMAGE_FILE_MACHINE_MIPSFPU16:
            strcpy(pszMeaning, "MIPSFPU16");
            break;

        case IMAGE_FILE_MACHINE_AXP64:  //IMAGE_FILE_MACHINE_ALPHA64
            strcpy(pszMeaning, "ALPHA64");
            break;
  
        case IMAGE_FILE_MACHINE_TRICORE:
            strcpy(pszMeaning, "Infineon TriCore");
            break;

        case IMAGE_FILE_MACHINE_CEF:
            strcpy(pszMeaning, "Infineon CEF");
            break;

        case IMAGE_FILE_MACHINE_EBC:
            strcpy(pszMeaning, "EFI Byte Code");
            break;

        case IMAGE_FILE_MACHINE_AMD64:
            strcpy(pszMeaning, "AMD64 (K8)");
            break;

        case IMAGE_FILE_MACHINE_M32R:
            strcpy(pszMeaning, "M32R");
            break;

        case IMAGE_FILE_MACHINE_ARM64:
            strcpy(pszMeaning, "ARM64");
            break;

        case IMAGE_FILE_MACHINE_CEE:
            strcpy(pszMeaning, "CEE");
            break;
        default:
            strcpy(pszMeaning, "");
            break;
    }
}

void MainWindow::SetCommonHeaderInfo(int n)
{
    //清除旧的表格数据，包括标题头
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    //设置n列标题头
    ui->tableWidget->setColumnCount(n);

    //设置标题头
    QStringList header;
    switch(n)
    {
        case 5:
            header << "Member" << "Offset" << "Size" << "Value" << "Meaning";
            break;
        case 4:
            header << "Member" << "Offset" << "Size" << "Value";
            break;
    }
    ui->tableWidget->setHorizontalHeaderLabels(header);
}

void MainWindow::AddInfo(int &nOffset, int &nRow, const char *pszName, int n)
{
    const char *pszFields[] = {"_member", "_offset", "_size", "_value", "_meaning"};
    QString member, offset, size, value, meaning;
    QTableWidgetItem* _member = nullptr;
    QTableWidgetItem* _offset = nullptr;
    QTableWidgetItem* _size = nullptr;
    QTableWidgetItem* _value = nullptr;
    QTableWidgetItem* _meaning = nullptr;


    char pDataType[10] = { 0 };
    char pszMeaning[20] = { 0 };
    char szBuf[MAX_PATH] = { 0 };
    //void *pVal = nullptr;
    DWORD dwVal = 0;

    //接收字段
    strcpy(szBuf, pszName);

    //添加到表格
    ui->tableWidget->insertRow(nRow);
    member = QString(szBuf);
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    //GetDataType(sizeof(m_pFH->Characteristics), pDataType);
    //GetDataTypeAndVal(IN "IMAGE....", IN (void*)m_pFH, IN szBuf, OUT dataType, OUT (void*)pVal);
    //GetDataTypeAndVal("IMAGE_OPTIONAL_HEADER32",
    //                  m_pFH,
    //                  szBuf,
    //                  pDataType,
    //                  pVal);

    GetDataTypeAndVal("IMAGE_OPTIONAL_HEADER32",
        m_pOH32,
        szBuf,
        pDataType,
        dwVal);

    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    //*(WORD*)pVal;

    if (strcmp(pDataType, "BYTE") == 0)
    {
        value = QString("%1").arg((BYTE)dwVal, 2, 16, QLatin1Char('0'));
    }
    else if (strcmp(pDataType, "WORD") == 0)
    {
        value = QString("%1").arg((WORD)dwVal, 4, 16, QLatin1Char('0'));
    }
    else if (strcmp(pDataType, "DWORD") == 0)
    {
        value = QString("%1").arg((DWORD)dwVal, 8, 16, QLatin1Char('0'));
    }
    _value = new QTableWidgetItem(value.toUpper());

    if (n=5)
    {
        //if (strcmp(pDataType, "BYTE") == 0)
        //{
        //    GetMeaning((BYTE)dwVal, pszMeaning);
        //}
        //else if (strcmp(pDataType, "WORD") == 0)
        //{
        //    GetMeaning((WORD)dwVal, pszMeaning);
        //}
        //else if (strcmp(pDataType, "DWORD") == 0)
        //{
        //    GetMeaning((DWORD)dwVal, pszMeaning);
        //}
        //meaning = QString(pszMeaning);
        //_meaning = new QTableWidgetItem(meaning);
    }

    QTableWidgetItem *pAry[5] = { _member , _offset, _size, _value,  _meaning };
    for (size_t i=0; i<n; i++)
    {
        ui->tableWidget->setItem(nRow, i, pAry[i]);
    }

    nRow++;

    if (strcmp(pDataType, "BYTE") == 0)
    {
        nOffset += sizeof(BYTE);
    }
    else if (strcmp(pDataType, "WORD") == 0)
    {
        nOffset += sizeof(WORD);
    }
    else if (strcmp(pDataType, "DWORD") == 0)
    {
        nOffset += sizeof(DWORD);
    }
}

void MainWindow::AddInfoEx(int& nOffset, int& nRow, const char* pszName, PVOID pxxHeader, const char* pszHeaderType, int n)
{
    const char* pszFields[] = { "_member", "_offset", "_size", "_value", "_meaning" };
    QString member, offset, size, value, meaning;
    QTableWidgetItem* _member = nullptr;
    QTableWidgetItem* _offset = nullptr;
    QTableWidgetItem* _size = nullptr;
    QTableWidgetItem* _value = nullptr;
    QTableWidgetItem* _meaning = nullptr;


    char pDataType[10] = { 0 };
    char pszMeaning[20] = { 0 };
    char szBuf[MAX_PATH] = { 0 };
    //void *pVal = nullptr;
    DWORD dwVal = 0;

    //接收字段
    strcpy(szBuf, pszName);

    //添加到表格
    if (strcmp(pszHeaderType, "IMAGE_EXPORT_DIRECTORY") == 0)
    {
        ui->tw_export_dir_1->insertRow(nRow);
    }
    if (strcmp(pszHeaderType, "IMAGE_TLS_DIRECTORY") == 0)
    {
        ui->tw_petls->insertRow(nRow);
    }

    member = QString(szBuf);
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    //GetDataType(sizeof(m_pFH->Characteristics), pDataType);
    //GetDataTypeAndVal(IN "IMAGE....", IN (void*)m_pFH, IN szBuf, OUT dataType, OUT (void*)pVal);
    //GetDataTypeAndVal("IMAGE_OPTIONAL_HEADER32",
    //                  m_pFH,
    //                  szBuf,
    //                  pDataType,
    //                  pVal);

    GetDataTypeAndVal(pszHeaderType,
        pxxHeader,
        szBuf,
        pDataType,
        dwVal);

    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    //*(WORD*)pVal;

    if (strcmp(pDataType, "BYTE") == 0)
    {
        value = QString("%1").arg((BYTE)dwVal, 2, 16, QLatin1Char('0'));
    }
    else if (strcmp(pDataType, "WORD") == 0)
    {
        value = QString("%1").arg((WORD)dwVal, 4, 16, QLatin1Char('0'));
    }
    else if (strcmp(pDataType, "DWORD") == 0)
    {
        value = QString("%1").arg((DWORD)dwVal, 8, 16, QLatin1Char('0'));
    }
    _value = new QTableWidgetItem(value.toUpper());

    //if (n = 5)
    //{
    //    //if (strcmp(pDataType, "BYTE") == 0)
    //    //{
    //    //    GetMeaning((BYTE)dwVal, pszMeaning);
    //    //}
    //    //else if (strcmp(pDataType, "WORD") == 0)
    //    //{
    //    //    GetMeaning((WORD)dwVal, pszMeaning);
    //    //}
    //    //else if (strcmp(pDataType, "DWORD") == 0)
    //    //{
    //    //    GetMeaning((DWORD)dwVal, pszMeaning);
    //    //}
    //    //meaning = QString(pszMeaning);
    //    //_meaning = new QTableWidgetItem(meaning);
    //}

    QTableWidgetItem* pAry[5] = { _member , _offset, _size, _value,  _meaning };
    if (strcmp(pszHeaderType, "IMAGE_EXPORT_DIRECTORY") == 0)
    {
        for (size_t i = 0; i < n; i++)
        {
            ui->tw_export_dir_1->setItem(nRow, i, pAry[i]);
        }
    }

    if (strcmp(pszHeaderType, "IMAGE_TLS_DIRECTORY") == 0)
    {
        for (size_t i = 0; i < n; i++)
        {
            ui->tw_petls->setItem(nRow, i, pAry[i]);
        }
    }

    nRow++;

    if (strcmp(pDataType, "BYTE") == 0)
    {
        nOffset += sizeof(BYTE);
    }
    else if (strcmp(pDataType, "WORD") == 0)
    {
        nOffset += sizeof(WORD);
    }
    else if (strcmp(pDataType, "DWORD") == 0)
    {
        nOffset += sizeof(DWORD);
    }
}

void MainWindow::AddSectionInfo(int nRow)
{
    QTableWidgetItem *pName = nullptr;
    QTableWidgetItem *pVirtualSize = nullptr;
    QTableWidgetItem *pVirtualAddress = nullptr;
    QTableWidgetItem *pRawSize = nullptr;
    QTableWidgetItem *pRawAddress = nullptr;
    QTableWidgetItem *pRelocAddress = nullptr;
    QTableWidgetItem *pLinenumbers = nullptr;
    QTableWidgetItem *pRelocationsNumber = nullptr;
    QTableWidgetItem *pLinenumbersNumber = nullptr;
    QTableWidgetItem *pcharacteristics = nullptr;

    QString name, virtualSize, virtualAddress,
            rawSize, rawAddress, relocAddress,
            lineNumbers, relocationsNumber, linenumbersNumber,
            characteristics;

    //插入一行
    ui->tw_pesh_1->insertRow(nRow);

    //添加的时候记录下来偏移
    int nOffset = (int)m_pSH - (int)m_pDH;

    //组织数据    
    name = QString((const char*)m_pSH->Name);
    pName = new QTableWidgetItem(name);

    virtualSize = QString("%1").arg(m_pSH->Misc.VirtualSize, 8, 16, QLatin1Char('0'));
    pVirtualSize = new QTableWidgetItem(virtualSize.toUpper());

    virtualAddress = QString("%1").arg(m_pSH->VirtualAddress, 8, 16, QLatin1Char('0'));
    pVirtualAddress = new QTableWidgetItem(virtualAddress.toUpper());

    rawSize = QString("%1").arg(m_pSH->SizeOfRawData, 8, 16, QLatin1Char('0'));
    pRawSize = new QTableWidgetItem(rawSize.toUpper());

    rawAddress = QString("%1").arg(m_pSH->PointerToRawData, 8, 16, QLatin1Char('0'));
    pRawAddress = new QTableWidgetItem(rawAddress.toUpper());

    relocAddress = QString("%1").arg(m_pSH->PointerToRelocations, 8, 16, QLatin1Char('0'));
    pRelocAddress = new QTableWidgetItem(relocAddress.toUpper());

    lineNumbers = QString("%1").arg(m_pSH->PointerToLinenumbers, 8, 16, QLatin1Char('0'));
    pLinenumbers = new QTableWidgetItem(lineNumbers.toUpper());

    relocationsNumber = QString("%1").arg(m_pSH->NumberOfRelocations, 8, 16, QLatin1Char('0'));
    pRelocationsNumber = new QTableWidgetItem(relocationsNumber.toUpper());

    linenumbersNumber = QString("%1").arg(m_pSH->NumberOfLinenumbers, 8, 16, QLatin1Char('0'));
    pLinenumbersNumber = new QTableWidgetItem(linenumbersNumber.toUpper());

    characteristics = QString("%1").arg(m_pSH->Characteristics, 8, 16, QLatin1Char('0'));
    pcharacteristics = new QTableWidgetItem(characteristics.toUpper());
    
    QTableWidgetItem *pAry[] = { pName, pVirtualSize, pVirtualAddress,
                    pRawSize, pRawAddress, pRelocAddress, 
                    pLinenumbers, pRelocationsNumber, pLinenumbersNumber,
                    pcharacteristics };
    //添加到表格
    for (size_t i=0; i<10; i++)
    {
        ui->tw_pesh_1->setItem(nRow, i, pAry[i]);
    }
}

void MainWindow::FormatSectionRetVal(IN PIMAGE_SECTION_HEADER pSH, IN const char* pszColumn, OUT DWORD & dwVal)
{
    if (strcmp(pszColumn, "Name") == 0)
    {
        dwVal = (DWORD)pSH->Name;
    }
    else if (strcmp(pszColumn, "VirtualSize") == 0)
    {
        dwVal = pSH->Misc.VirtualSize;
    }
    else if (strcmp(pszColumn, "VirtualAddress") == 0)
    {
        dwVal = pSH->VirtualAddress;
    }
    else if (strcmp(pszColumn, "SizeOfRawData") == 0)
    {
        dwVal = pSH->SizeOfRawData;
    }
    else if (strcmp(pszColumn, "PointerToLinenumbers") == 0)
    {
        dwVal = pSH->PointerToLinenumbers;
    }
    else if (strcmp(pszColumn, "NumberOfRelocations") == 0)
    {
        dwVal = pSH->NumberOfRelocations;
    }
    else if (strcmp(pszColumn, "NumberOfLinenumbers") == 0)
    {
        dwVal = pSH->NumberOfLinenumbers;
    }
    else if (strcmp(pszColumn, "Characteristics") == 0)
    {
        dwVal = pSH->Characteristics;
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent * event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent * event)
{  
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clear();
    ui->tableWidget->update();
    if (m_hFileRead != NULL)
    {
        CloseHandle(m_hFileRead);
    }
    if (m_hMapFile != NULL)
    {
        CloseHandle(m_hMapFile);
    }

    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    m_filePath = urls.first().toLocalFile();
    if (m_filePath.isEmpty()) {
        return;
    }
    qDebug() << m_filePath << '\n';

    //打开exe
    if (MyOpenFile() == -1)
        return;
       
    //获取模块基地址
    if (!GetImageBase())
        return;

    //是否是PE文件
    if (!IsPE())
        return;

    //默认显示DOS Header
    ShowDosHeaderInfo();
}

int MainWindow::MyOpenFile()
{
    m_hFileRead = CreateFileA(m_filePath.toLatin1().data(),              //要打开的文件名
                              GENERIC_READ,            //以读方式打开
                              0,                       //不可共享读写
                              NULL,                    //默认安全属性
                              OPEN_EXISTING,           //只打开已经存在的文件
                              FILE_ATTRIBUTE_NORMAL,   //常规文件属性
                              NULL);                   //无模板

    //打开文件是否成功
    if (m_hFileRead == INVALID_HANDLE_VALUE)
    {
        GetErrorMsg();
        return -1;
    }

    //HANDLE hMapFile;
    //映射文件
    m_hMapFile = CreateFileMapping(m_hFileRead,      //需要映射的文件的句柄
                                   NULL,           //安全选项：默认
                                   PAGE_READONLY,  //只读
                                   0,              //mapping对象的大小，高位
                                   0,              //mapping对象的大小，低位
                                   NULL);          //mapping对象的名字
    if (m_hMapFile == INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hFileRead);
        GetErrorMsg();
        return -1;
    }

    return 0;
}

BOOL MainWindow::GetImageBase()
{
    m_hImageBase = MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0, 0);
    if (!m_hImageBase)
    {
        GetErrorMsg();

        CloseHandle(m_hMapFile);
        CloseHandle(m_hFileRead);

        return FALSE;
    }
    return TRUE;
}

BOOL MainWindow::IsPE()
{
    //MZ
    m_pDH = (PIMAGE_DOS_HEADER)m_hImageBase;

    if (!m_pDH)
        return FALSE;


    if (m_pDH->e_magic != IMAGE_DOS_SIGNATURE)
    {
        CloseHandle(m_hMapFile);
        CloseHandle(m_hFileRead);
        return FALSE;
    }
        
    //PE
    m_pNTH = (PIMAGE_NT_HEADERS32)((DWORD)m_pDH + m_pDH->e_lfanew);
    if (m_pNTH->Signature != IMAGE_NT_SIGNATURE)
    {
        CloseHandle(m_hMapFile);
        CloseHandle(m_hFileRead);
        return FALSE;
    }
    
    m_oldFilePath = m_filePath;

    return TRUE;
}

void MainWindow::ShowDosHeaderInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->page_NH);

    int nRow = 0; //行
    //char *pOffset = 0; //偏移，累加
    int nOffset = 0;
    char pDataType[10] = { 0 };
    //cout << "  e_magic:" << hex << m_pDH->e_magic << "offset "<< static_cast<const void *>(pszStr) << sizeof(m_pDH->e_magic) << endl;
    //清除旧数据
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
    ui->tableWidget->setColumnCount(4);//设置4列

        //新的标题头
    QStringList header;
    header << "Member" << "Offset" << "Size" << "Value";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    //添加到表格
    //格式：Member Offset Size Value
    QString member, offset, size, value;
    QTableWidgetItem* _member = nullptr;
    QTableWidgetItem* _offset = nullptr; 
    QTableWidgetItem* _size = nullptr;
    QTableWidgetItem* _value = nullptr;

    //e_magic
    ui->tableWidget->insertRow(nRow);
    member = QString("e_magic");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_magic), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_magic, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_magic);

    //e_cblp
    ui->tableWidget->insertRow(nRow);
    member = QString("e_cblp");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_cblp), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_cblp, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_cblp);

    //e_cp
    ui->tableWidget->insertRow(nRow);
    member = QString("e_cp");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_cp), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_cp, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_cp);

    //e_crlc
    ui->tableWidget->insertRow(nRow);
    member = QString("e_crlc");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_crlc), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_crlc, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_crlc);

    //e_cparhdr
    ui->tableWidget->insertRow(nRow);
    member = QString("e_cparhdr");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_cparhdr), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_cparhdr, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_cparhdr);

    //e_minalloc
    ui->tableWidget->insertRow(nRow);
    member = QString("e_minalloc");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_minalloc), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_minalloc, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_minalloc);

    //e_maxalloc
    ui->tableWidget->insertRow(nRow);
    member = QString("e_maxalloc");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_maxalloc), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_maxalloc, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_maxalloc);

    //e_ss
    ui->tableWidget->insertRow(nRow);
    member = QString("e_ss");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_ss), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_ss, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_ss);

    //e_sp
    ui->tableWidget->insertRow(nRow);
    member = QString("e_sp");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_sp), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_sp, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_sp);

    //e_csum
    ui->tableWidget->insertRow(nRow);
    member = QString("e_csum");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_csum), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_csum, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_csum);

    //e_ip
    ui->tableWidget->insertRow(nRow);
    member = QString("e_ip");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_ip), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_ip, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_ip);

    //e_cs
    ui->tableWidget->insertRow(nRow);
    member = QString("e_cs");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_cs), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_cs, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_cs);

    //e_lfarlc
    ui->tableWidget->insertRow(nRow);
    member = QString("e_lfarlc");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_lfarlc), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_lfarlc, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_lfarlc);

    //e_ovno
    ui->tableWidget->insertRow(nRow);
    member = QString("e_ovno");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_ovno), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_ovno, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_ovno);

    //e_res
    for (size_t i=0; i<sizeof(m_pDH->e_res)/sizeof(m_pDH->e_res[0]); i++)
    {
        ui->tableWidget->insertRow(nRow);
        member = QString("e_res");
        _member = new QTableWidgetItem(member);
        offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
        _offset = new QTableWidgetItem(offset);
        GetDataType(sizeof(m_pDH->e_res), pDataType);
        size = QString(pDataType);
        _size = new QTableWidgetItem(size);
        value = QString(QString::number(m_pDH->e_res[i], 16));
        _value = new QTableWidgetItem(value.toUpper());
        if (i==0)
        {
            ui->tableWidget->setItem(nRow, 0, _member);
        }
        ui->tableWidget->setItem(nRow, 1, _offset);
        ui->tableWidget->setItem(nRow, 2, _size);
        ui->tableWidget->setItem(nRow, 3, _value);
        nRow++;
        nOffset += sizeof(m_pDH->e_res[i]);
    }

    //e_oemid
    ui->tableWidget->insertRow(nRow);
    member = QString("e_oemid");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_oemid), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_oemid, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_oemid);

    //e_oeminfo
    ui->tableWidget->insertRow(nRow);
    member = QString("e_oeminfo");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset);
    GetDataType(sizeof(m_pDH->e_oeminfo), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_oeminfo, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_oeminfo);

    //e_res2
    for (size_t i = 0; i < sizeof(m_pDH->e_res2) / sizeof(m_pDH->e_res2[0]); i++)
    {
        ui->tableWidget->insertRow(nRow);
        member = QString("e_res2");
        _member = new QTableWidgetItem(member);
        offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
        _offset = new QTableWidgetItem(offset);
        GetDataType(sizeof(m_pDH->e_res2), pDataType);
        size = QString(pDataType);
        _size = new QTableWidgetItem(size);
        value = QString(QString::number(m_pDH->e_res2[i], 16));
        _value = new QTableWidgetItem(value.toUpper());
        if (i == 0)
        {
            ui->tableWidget->setItem(nRow, 0, _member);
        }
        ui->tableWidget->setItem(nRow, 1, _offset);
        ui->tableWidget->setItem(nRow, 2, _size);
        ui->tableWidget->setItem(nRow, 3, _value);
        nRow++;
        nOffset += sizeof(m_pDH->e_res2[i]);
    }

    //e_lfanew
    ui->tableWidget->insertRow(nRow);
    member = QString("e_lfanew");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    GetDataType(sizeof(m_pDH->e_lfanew), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pDH->e_lfanew, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pDH->e_lfanew);
}

void MainWindow::ShowNtHeadersInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->page_NH);
    int nRow = 0; //行
    //char *pOffset = 0; //偏移，累加
    int nOffset = m_pDH->e_lfanew;
    char pDataType[10] = { 0 };
    //cout << "  e_magic:" << hex << m_pDH->e_magic << "offset "<< static_cast<const void *>(pszStr) << sizeof(m_pDH->e_magic) << endl;
    //清除旧数据，不包括标题头
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
    ui->tableWidget->setColumnCount(4);//设置4列

        //新的标题头
    QStringList header;
    header << "Member" << "Offset" << "Size" << "Value";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    //添加到表格
    //格式：Member Offset Size Value
    QString member, offset, size, value;
    QTableWidgetItem* _member = nullptr;
    QTableWidgetItem* _offset = nullptr;
    QTableWidgetItem* _size = nullptr;
    QTableWidgetItem* _value = nullptr;

    //Signature
    ui->tableWidget->insertRow(nRow);
    member = QString("Signature");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    GetDataType(sizeof(m_pNTH->Signature), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pNTH->Signature, 16));
    _value = new QTableWidgetItem(value.toUpper());
    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    nRow++;
    nOffset += sizeof(m_pNTH->Signature);
}

void MainWindow::ShowFileHeaderInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->page_NH);
    int nRow = 0; //行
    char pDataType[10] = { 0 };//数据类型
    char pszMeaning[20] = { 0 };//字段含义
    int nOffset = m_pDH->e_lfanew + sizeof(m_pNTH->Signature); //偏移

    m_pFH = &m_pNTH->FileHeader;

    //清除旧的表格数据，包括标题头
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(5);//设置5列

    //新的标题头
    QStringList header;
    header << "Member" << "Offset" << "Size" << "Value" << "Meaning";
    ui->tableWidget->setHorizontalHeaderLabels(header);

    //添加到表格
    //格式：Member Offset Size Value
    QString member, offset, size, value, meaning;
    QTableWidgetItem* _member = nullptr;
    QTableWidgetItem* _offset = nullptr;
    QTableWidgetItem* _size = nullptr;
    QTableWidgetItem* _value = nullptr;
    QTableWidgetItem* _meaning = nullptr;

    //Machine
    ui->tableWidget->insertRow(nRow);
    member = QString("Machine");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    GetDataType(sizeof(m_pFH->Machine), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pFH->Machine, 16));
    _value = new QTableWidgetItem(value.toUpper());
    GetMeaning(m_pFH->Machine, pszMeaning);
    meaning = QString(pszMeaning);
    _meaning = new QTableWidgetItem(meaning);

    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    ui->tableWidget->setItem(nRow, 4, _meaning);
    nRow++;
    nOffset += sizeof(m_pFH->Machine);

    //NumberOfSections
    ui->tableWidget->insertRow(nRow);
    member = QString("NumberOfSections");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    GetDataType(sizeof(m_pFH->NumberOfSections), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pFH->NumberOfSections, 16));
    _value = new QTableWidgetItem(value.toUpper());
    GetMeaning(m_pFH->NumberOfSections, pszMeaning);
    meaning = QString(pszMeaning);
    _meaning = new QTableWidgetItem(meaning);

    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    ui->tableWidget->setItem(nRow, 4, _meaning);
    nRow++;
    nOffset += sizeof(m_pFH->NumberOfSections);

    //TimeDateStamp
    ui->tableWidget->insertRow(nRow);
    member = QString("TimeDateStamp");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    GetDataType(sizeof(m_pFH->TimeDateStamp), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pFH->TimeDateStamp, 16));
    _value = new QTableWidgetItem(value.toUpper());
    GetMeaning(m_pFH->TimeDateStamp, pszMeaning);
    meaning = QString(pszMeaning);
    _meaning = new QTableWidgetItem(meaning);

    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    ui->tableWidget->setItem(nRow, 4, _meaning);
    nRow++;
    nOffset += sizeof(m_pFH->TimeDateStamp);

    //PointerToSymbolTable
    ui->tableWidget->insertRow(nRow);
    member = QString("PointerToSymbolTable");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    GetDataType(sizeof(m_pFH->PointerToSymbolTable), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pFH->PointerToSymbolTable, 16));
    _value = new QTableWidgetItem(value.toUpper());
    GetMeaning(m_pFH->PointerToSymbolTable, pszMeaning);
    meaning = QString(pszMeaning);
    _meaning = new QTableWidgetItem(meaning);

    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    ui->tableWidget->setItem(nRow, 4, _meaning);
    nRow++;
    nOffset += sizeof(m_pFH->PointerToSymbolTable);

    //NumberOfSymbols
    ui->tableWidget->insertRow(nRow);
    member = QString("NumberOfSymbols");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    GetDataType(sizeof(m_pFH->NumberOfSymbols), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pFH->NumberOfSymbols, 16));
    _value = new QTableWidgetItem(value.toUpper());
    GetMeaning(m_pFH->NumberOfSymbols, pszMeaning);
    meaning = QString(pszMeaning);
    _meaning = new QTableWidgetItem(meaning);

    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    ui->tableWidget->setItem(nRow, 4, _meaning);
    nRow++;
    nOffset += sizeof(m_pFH->NumberOfSymbols);

    //SizeOfOptionalHeader
    ui->tableWidget->insertRow(nRow);
    member = QString("SizeOfOptionalHeader");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    GetDataType(sizeof(m_pFH->SizeOfOptionalHeader), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pFH->SizeOfOptionalHeader, 16));
    _value = new QTableWidgetItem(value.toUpper());
    GetMeaning(m_pFH->SizeOfOptionalHeader, pszMeaning);
    meaning = QString(pszMeaning);
    _meaning = new QTableWidgetItem(meaning);

    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    ui->tableWidget->setItem(nRow, 4, _meaning);
    nRow++;
    nOffset += sizeof(m_pFH->SizeOfOptionalHeader);

    //Characteristics
    ui->tableWidget->insertRow(nRow);
    member = QString("Characteristics");
    _member = new QTableWidgetItem(member);
    offset = QString("%1").arg(nOffset, 8, 16, QLatin1Char('0'));
    _offset = new QTableWidgetItem(offset.toUpper());
    GetDataType(sizeof(m_pFH->Characteristics), pDataType);
    size = QString(pDataType);
    _size = new QTableWidgetItem(size);
    value = QString(QString::number(m_pFH->Characteristics, 16));
    _value = new QTableWidgetItem(value.toUpper());
    GetMeaning(m_pFH->Characteristics, pszMeaning);
    meaning = QString(pszMeaning);
    _meaning = new QTableWidgetItem(meaning);

    ui->tableWidget->setItem(nRow, 0, _member);
    ui->tableWidget->setItem(nRow, 1, _offset);
    ui->tableWidget->setItem(nRow, 2, _size);
    ui->tableWidget->setItem(nRow, 3, _value);
    ui->tableWidget->setItem(nRow, 4, _meaning);
    nRow++;
    nOffset += sizeof(m_pFH->Characteristics);
}

void MainWindow::ShowOptionalHeaderInfo()
{
    m_pOH32 = &m_pNTH->OptionalHeader;

    cout << m_pOH32->Magic << '\n';

    SetCommonHeaderInfo(5);

    int nRow = 0;
    int nOffset = (DWORD)m_pOH32 - (DWORD)m_pDH;
    //int nOffset, char *pszName, int n = 4
    AddInfo(nOffset,nRow, "Magic");
    AddInfo(nOffset,nRow, "MajorLinkerVersion");
    AddInfo(nOffset,nRow, "MinorLinkerVersion");
    AddInfo(nOffset,nRow, "SizeOfCode");
    AddInfo(nOffset,nRow, "SizeOfInitializedData");
    AddInfo(nOffset,nRow, "SizeOfUninitializedData");
    AddInfo(nOffset,nRow, "AddressOfEntryPoint");
    AddInfo(nOffset,nRow, "BaseOfCode");
    AddInfo(nOffset,nRow, "BaseOfData");
    AddInfo(nOffset,nRow, "ImageBase");
    AddInfo(nOffset,nRow, "SectionAlignment");
    AddInfo(nOffset,nRow, "FileAlignment");
    AddInfo(nOffset,nRow, "MajorOperatingSystemVersion");
    AddInfo(nOffset,nRow, "MinorOperatingSystemVersion");
    AddInfo(nOffset,nRow, "MajorImageVersion");
    AddInfo(nOffset,nRow, "MinorImageVersion");
    AddInfo(nOffset,nRow, "MajorSubsystemVersion");
    AddInfo(nOffset,nRow, "MinorSubsystemVersion");
    AddInfo(nOffset,nRow, "Win32VersionValue");
    AddInfo(nOffset,nRow, "SizeOfImage");
    AddInfo(nOffset,nRow, "SizeOfHeaders");
    AddInfo(nOffset,nRow, "CheckSum");
    AddInfo(nOffset,nRow, "Subsystem");
    AddInfo(nOffset,nRow, "DllCharacteristics");
    AddInfo(nOffset,nRow, "SizeOfStackReserve");
    AddInfo(nOffset,nRow, "SizeOfStackCommit");
    AddInfo(nOffset,nRow, "SizeOfHeapReserve");
    AddInfo(nOffset,nRow, "SizeOfHeapCommit");
    AddInfo(nOffset,nRow, "LoaderFlags");
    AddInfo(nOffset,nRow, "NumberOfRvaAndSizes");
}

void MainWindow::ShowDataDirectoriesXInfo()
{
}

void MainWindow::ShowSectionDirectoryXInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->page_SH);

    ui->tw_pesh_1->setRowCount(0);
    ui->tw_pesh_1->clearContents();
    
    //获取第一个节表位置
    GetFirstSectionHeader();

    //获取文件头
    m_pFH = &m_pNTH->FileHeader;
    if (!m_pFH)
    {
        GetErrorMsg();
        return;
    }

    //遍历节表
    for (size_t i = 0; i < m_pFH->NumberOfSections; i++)
    {
        //显示到界面
        AddSectionInfo(i);
        ++m_pSH;
    }
}

void MainWindow::GetFirstSectionHeader()
{
    //获取节表头
    m_pSH = IMAGE_FIRST_SECTION(m_pNTH);
}

/************************************************************************/
/*
功能:虚拟内存相对地址和文件偏移的转换
参数：stRVA：    虚拟内存相对偏移地址
lpFileBuf: 文件起始地址
返回：转换后的文件偏移地址
*/
/************************************************************************/
size_t MainWindow::RVAToOffset(size_t stRVA, PVOID lpFileBuf)
{
    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpFileBuf;
    size_t stPEHeadAddr = (size_t)lpFileBuf + pDos->e_lfanew;
    PIMAGE_NT_HEADERS32 pNT = (PIMAGE_NT_HEADERS32)stPEHeadAddr;
    //区段数
    DWORD dwSectionCount = pNT->FileHeader.NumberOfSections;
    //内存对齐大小
    DWORD dwMemoruAil = pNT->OptionalHeader.SectionAlignment;
    PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNT);
    //距离命中节的起始虚拟地址的偏移值。
    DWORD  dwDiffer = 0;
    for (DWORD i = 0; i < dwSectionCount; i++)
    {
        //模拟内存对齐机制
        DWORD dwBlockCount = pSection[i].SizeOfRawData / dwMemoruAil;
        dwBlockCount += pSection[i].SizeOfRawData%dwMemoruAil ? 1 : 0;

        DWORD dwBeginVA = pSection[i].VirtualAddress;
        DWORD dwEndVA = pSection[i].VirtualAddress + dwBlockCount * dwMemoruAil;
        //如果stRVA在某个区段中
        if (stRVA >= dwBeginVA && stRVA < dwEndVA)
        {
            dwDiffer = stRVA - dwBeginVA;
            return pSection[i].PointerToRawData + dwDiffer;
        }
        else if (stRVA < dwBeginVA)//在文件头中直接返回
        {
            //return stRVA;
            return -1;
        }
    }
    return 0;
}

/************************************************************************/
/*
功能：文件偏移地址和虚拟地址的转换
参数：stOffset：文件偏移地址
lpFileBuf:虚拟内存起始地址
返回：转换后的虚拟地址
*/
/************************************************************************/
size_t MainWindow::Offset2VA(size_t stOffset, PVOID lpFileBuf)
{
    //获取DOS头
    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpFileBuf;
    //获取PE头
    //e_lfanew:PE头相对于文件的偏移地址
    size_t stPEHeadAddr = (size_t)lpFileBuf + pDos->e_lfanew;
    PIMAGE_NT_HEADERS32 pNT = (PIMAGE_NT_HEADERS32)stPEHeadAddr;
    //区段数
    DWORD dwSectionCount = pNT->FileHeader.NumberOfSections;
    //映像地址
    DWORD dwImageBase = pNT->OptionalHeader.ImageBase;
    //区段头
    PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNT);

    //相对大小
    DWORD  dwDiffer = 0;
    for (DWORD i = 0; i < dwSectionCount; i++)
    {
        //区段的起始地址和结束地址
        DWORD dwBeginVA = pSection[i].PointerToRawData;
        DWORD dwEndVA = pSection[i].PointerToRawData + pSection[i].SizeOfRawData;
        //如果文件偏移地址在dwBeginVA和dwEndVA之间
        if (stOffset >= dwBeginVA && stOffset < dwEndVA)
        {
            //相对大小
            dwDiffer = stOffset - dwBeginVA;
            //进程的起始地址 + 区段的相对地址 + 相对区段的大小
            return dwImageBase + pSection[i].VirtualAddress + dwDiffer;
        }
        else if (stOffset < dwBeginVA)    //如果文件偏移地址不在区段中
        {
            return dwImageBase + stOffset;
        }
    }
    return 0;
}

void MainWindow::ShowImportDirectoryInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ID);

    //设置第2行标题
    ui->tw_peid_1->setItem(1, 0, new QTableWidgetItem(tr("szAnsi")));
    ui->tw_peid_1->setItem(1, 1, new QTableWidgetItem(tr("(nFunctions)")));
    ui->tw_peid_1->setItem(1, 2, new QTableWidgetItem(tr("Dword")));
    ui->tw_peid_1->setItem(1, 3, new QTableWidgetItem(tr("Dword")));
    ui->tw_peid_1->setItem(1, 4, new QTableWidgetItem(tr("Dword")));
    ui->tw_peid_1->setItem(1, 5, new QTableWidgetItem(tr("Dword")));
    ui->tw_peid_1->setItem(1, 6, new QTableWidgetItem(tr("Dword")));

    //取得入口地址所需参数
    PIMAGE_DOS_HEADER pDH = NULL;
    PIMAGE_NT_HEADERS pNTH = NULL;
    PIMAGE_OPTIONAL_HEADER pOH = NULL;
    PIMAGE_IMPORT_DESCRIPTOR pImportDir = NULL;
    DWORD dwImportDirRVA = 0;
    DWORD dwImportDirFOA = 0;

    pDH = (PIMAGE_DOS_HEADER)m_hImageBase;
    pNTH = (PIMAGE_NT_HEADERS)((DWORD)pDH + pDH->e_lfanew);
    pOH = &pNTH->OptionalHeader;
    //导入表RVA
    dwImportDirRVA = pOH->DataDirectory[1].VirtualAddress;

    //RVA转换得到FOA
    dwImportDirFOA = RVAToOffset(dwImportDirRVA, m_hImageBase);
    //得到内存中导入表地址
    pImportDir = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)m_hImageBase + dwImportDirFOA);

    //从第二行开始添加
    size_t nRow = 2;

    //遍历导入表数据
    while (pImportDir->FirstThunk)
    {
        //Module Name, Imports, OFTs, TimeDateStamp, ForwarderChain, Name RVA, FTs(IAT)

        ui->tw_peid_1->insertRow(nRow);
        
        //Module Name
        DWORD dwDllNameFOA;
        dwDllNameFOA = RVAToOffset(pImportDir->Name, m_hImageBase);
        char szDllName[256] = { 0 };
        char* pszDllName = NULL;
        pszDllName = (char*)((DWORD)m_hImageBase + dwDllNameFOA);
        strcpy(szDllName, pszDllName);

        QString qstrCol;
        QTableWidgetItem *item = NULL;
        //char* 转 QString
        qstrCol = QString(szDllName);
        item = new QTableWidgetItem(qstrCol);
        ui->tw_peid_1->setItem(nRow, 0, item);

        //Imports
        DWORD dwIAT_FOA = 0;
        //IAT RVA 转换得到 IAT FOA
        dwIAT_FOA = RVAToOffset(pImportDir->FirstThunk, m_hImageBase);
        //取得内存中IAT地址
        DWORD* pdwIDThunk = NULL;
        pdwIDThunk = (DWORD*)((DWORD)m_hImageBase + dwIAT_FOA);
        //遍历IAT，获取导入函数个数
        size_t nNumImpFunc = 0;

        while (*pdwIDThunk)
        {
            nNumImpFunc++;
            ++pdwIDThunk;
        }

        qstrCol = QString("%1").arg(nNumImpFunc);
        item = new QTableWidgetItem(qstrCol);
        ui->tw_peid_1->setItem(nRow, 1, item);

        //OFTs
        qstrCol = QString("%1").arg(pImportDir->OriginalFirstThunk, 8, 16, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_peid_1->setItem(nRow, 2, item);

        //TimeDateStamp
        qstrCol = QString("%1").arg(pImportDir->TimeDateStamp, 8, 16, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_peid_1->setItem(nRow, 3, item);

        //ForwarderChain
        qstrCol = QString("%1").arg(pImportDir->ForwarderChain, 8, 16, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_peid_1->setItem(nRow, 4, item);

        //Name RVA
        qstrCol = QString("%1").arg(pImportDir->Name, 8, 16, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_peid_1->setItem(nRow, 5, item);

        //FTs(IAT)
        qstrCol = QString("%1").arg(pImportDir->FirstThunk, 8, 16, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_peid_1->setItem(nRow, 6, item);
        
        ++nRow;
        ++pImportDir;
    }
}

void MainWindow::ShowRelocationDirectoryInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->page_RD);

    //设置第2行标题
    ui->tw_perd_1->setItem(1, 0, new QTableWidgetItem(tr("DWORD")));
    ui->tw_perd_1->setItem(1, 1, new QTableWidgetItem(tr("DWORD")));
    ui->tw_perd_1->setItem(1, 2, new QTableWidgetItem(tr("N/A")));

    //定位到重定位表
    PIMAGE_DOS_HEADER pDosHeader = NULL;
    PIMAGE_NT_HEADERS pNTHeader = NULL;
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;
    PIMAGE_BASE_RELOCATION pRelocationDir = NULL;
    DWORD dwRelocationDirRVA = 0;
    DWORD dwRelocationDirFOA = 0;

    pDosHeader = (PIMAGE_DOS_HEADER)m_hImageBase;
    pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
    pOptionalHeader = &pNTHeader->OptionalHeader;

    dwRelocationDirRVA = pOptionalHeader->DataDirectory[5].VirtualAddress;
    dwRelocationDirFOA = RVAToOffset(dwRelocationDirRVA, m_hImageBase);
    pRelocationDir = (PIMAGE_BASE_RELOCATION)((DWORD)m_hImageBase + dwRelocationDirFOA);

    //解析重定位表
    //RVA, Size Of Block, Items
    size_t nRow = 2;
    DWORD dwCurPageRelocFOA = dwRelocationDirFOA;
    while (pRelocationDir->VirtualAddress)
    {
        DWORD dwRelocRVA = 0;
        DWORD dwSizeOfBlock = 0;
        DWORD dwItems = 0;

        QString qstrCol;
        QTableWidgetItem *item = NULL;

        dwRelocRVA = pRelocationDir->VirtualAddress;
        dwSizeOfBlock = pRelocationDir->SizeOfBlock;
        dwItems = (dwSizeOfBlock - 8) / 2;

        ui->tw_perd_1->insertRow(nRow);
        //RVA
        qstrCol = QString("%1").arg(dwRelocRVA, 8, 16, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_perd_1->setItem(nRow, 0, item);

        //Size Of Block
        qstrCol = QString("%1").arg(dwSizeOfBlock, 8, 16, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_perd_1->setItem(nRow, 1, item);

        //Items
        qstrCol = QString("%1").arg(dwItems);
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_perd_1->setItem(nRow, 2, item);

        //进行下一页的判断
        pRelocationDir = (PIMAGE_BASE_RELOCATION)((BYTE*)pRelocationDir + dwSizeOfBlock);

        ++nRow;
    }
}

//当前页，重定位项的RVA地址
void MainWindow::QueryRelocationItems()
{
    ui->tw_perd_2->setRowCount(0);
    ui->tw_perd_2->clearContents();
    ui->tw_perd_2->insertRow(0);
    ui->tw_perd_2->insertRow(1);

    //取得当前页，重定位项RVA地址
    size_t stCurRow = ui->tw_perd_1->currentRow();
    if (stCurRow < 2)
        return;

    //添加标题项
    ui->tw_perd_2->setItem(1, 0, new QTableWidgetItem(tr("Word")));
    ui->tw_perd_2->setItem(1, 1, new QTableWidgetItem(tr("N/A")));
    ui->tw_perd_2->setItem(1, 2, new QTableWidgetItem(tr("N/A")));
    
    //定位到重定位表
    PIMAGE_DOS_HEADER pDosHeader = NULL;
    PIMAGE_NT_HEADERS pNTHeader = NULL;
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;
    PIMAGE_BASE_RELOCATION pRelocationDir = NULL;
    DWORD dwRelocationDirRVA = 0;
    DWORD dwRelocationDirFOA = 0;

    pDosHeader = (PIMAGE_DOS_HEADER)m_hImageBase;
    pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
    pOptionalHeader = &pNTHeader->OptionalHeader;

    dwRelocationDirRVA = pOptionalHeader->DataDirectory[5].VirtualAddress;
    //首页重定位页FOA
    dwRelocationDirFOA = RVAToOffset(dwRelocationDirRVA, m_hImageBase);  //A00
    pRelocationDir = (PIMAGE_BASE_RELOCATION)((DWORD)dwRelocationDirFOA + (DWORD)m_hImageBase);

    bool ok;
    DWORD dwCurLineVirtualAddress = 0;
    QString qstrHexRVA = 0;
    qstrHexRVA = ui->tw_perd_1->item(stCurRow, 0)->text();
    dwCurLineVirtualAddress = qstrHexRVA.toInt(&ok, 16); // 表示以16进制方式读取字符串

    DWORD dwCurPageRelocRVA = 0;
    DWORD dwCurPageRelocFOA = 0;
    DWORD dwSizeOfBlock_FOA = 0;
    dwCurPageRelocFOA = dwRelocationDirFOA;
    DWORD dwCurPageSizeOfBlock = 0;
    while (dwCurPageRelocRVA = pRelocationDir->VirtualAddress)
    {
        dwCurPageSizeOfBlock = pRelocationDir->SizeOfBlock;
        if (dwCurPageRelocRVA != dwCurLineVirtualAddress)
        {
            //计算后面重定位页
            pRelocationDir = (PIMAGE_BASE_RELOCATION)((BYTE*)pRelocationDir + dwCurPageSizeOfBlock);
            dwCurPageRelocFOA += dwCurPageSizeOfBlock; //包括IMAGE_BASE_RELOCATION
            continue;
        }

        break;
    }

    DWORD dwRelocRVA = 0;
    DWORD dwItems = 0;

    dwRelocRVA = pRelocationDir->VirtualAddress;
    dwItems = (dwCurPageSizeOfBlock - 8) / 2;

    WORD *pRelocItems = (WORD*)((BYTE*)pRelocationDir + sizeof(IMAGE_BASE_RELOCATION));
    size_t stRow = 2;
    //遍历重定位项
    for (DWORD i=0; i< dwItems; i++)
    {
        //Word      N/A       N/A
        //3030   0001030    HIGHLOW
        WORD nwItem = 0;
        DWORD dwPageRVA = 0;
        WORD nwType = 0;

        nwItem = pRelocItems[i];
        dwPageRVA = dwRelocRVA + (nwItem & 0x0fff);
        nwType = nwItem >> 12;

        //显示
        QString qstrCol;
        QTableWidgetItem *item = NULL;
        ui->tw_perd_2->insertRow(stRow);
        //Word
        qstrCol = QString("%1").arg(nwItem, 4, 16, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_perd_2->setItem(stRow, 0, item);
        //N/A
        qstrCol = QString("%1").arg(dwPageRVA, 8, 16, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_perd_2->setItem(stRow, 1, item);
        //N/A
        qstrCol = QString("%1").arg(nwType, 4, 8, QLatin1Char('0'));
        item = new QTableWidgetItem(qstrCol.toUpper());
        ui->tw_perd_2->setItem(stRow, 2, item);

        ++stRow;
    }
}

void MainWindow::ShowExportDirInfo()
{
    //初始化界面
    InitExportDir();
    
    //定位到导出表
    PIMAGE_DOS_HEADER pDosHeader = NULL;
    PIMAGE_NT_HEADERS pNTHeader = NULL;
    PIMAGE_OPTIONAL_HEADER pOptHeader = NULL;
    PIMAGE_EXPORT_DIRECTORY pExportDir = NULL;
    DWORD dwExportDirRVA = 0;
    DWORD dwExportDirFOA = 0;

    pDosHeader = (PIMAGE_DOS_HEADER)m_hImageBase;
    pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
    pOptHeader = &pNTHeader->OptionalHeader;
    dwExportDirRVA = pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    dwExportDirFOA = RVAToOffset(dwExportDirRVA, m_hImageBase);
    pExportDir = (PIMAGE_EXPORT_DIRECTORY)((DWORD)m_hImageBase + dwExportDirFOA);

    //解析导出表成员
    AddExportDirMember(pExportDir, dwExportDirFOA);

    //解析导出函数
    AddExportDirFunc(pExportDir);
}

void MainWindow::InitExportDir()
{
    //设置页面
    ui->stackedWidget->setCurrentWidget(ui->page_Export_Directory);
    //清理
    ui->tw_export_dir_1->setRowCount(0);
    ui->tw_export_dir_1->clearContents();
    ui->tw_export_dir_2->setRowCount(0);
    ui->tw_export_dir_2->clearContents();

    //添加表头
    ui->tw_export_dir_2->insertRow(0);
    ui->tw_export_dir_2->insertRow(1);
    ui->tw_export_dir_2->setItem(1, 0, new QTableWidgetItem(tr("(nFunctions)")));
    ui->tw_export_dir_2->setItem(1, 1, new QTableWidgetItem(tr("Dword")));
    ui->tw_export_dir_2->setItem(1, 2, new QTableWidgetItem(tr("Word")));
    ui->tw_export_dir_2->setItem(1, 3, new QTableWidgetItem(tr("Dword")));
    ui->tw_export_dir_2->setItem(1, 4, new QTableWidgetItem(tr("szAnsi")));
}

void MainWindow::AddExportDirMember(PVOID p, DWORD dwFOA)
{
    int nBaseOffset = dwFOA;
    int nRow = 0;
    //int nOffset, char *pszName, int n = 4
    //PIMAGE_EXPORT_DIRECTORY pExportDir = (PIMAGE_EXPORT_DIRECTORY)p;
    //pExportDir->AddressOfNameOrdinals
    AddInfoEx(nBaseOffset, nRow, "Characteristics", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "TimeDateStamp", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "MajorVersion", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "MinorVersion", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "Name", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "Base", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "NumberOfFunctions", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "NumberOfNames", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "AddressOfFunctions", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "AddressOfNames", p, "IMAGE_EXPORT_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "AddressOfNameOrdinals", p, "IMAGE_EXPORT_DIRECTORY");
}

void MainWindow::FormatExportDirMember(IN PIMAGE_EXPORT_DIRECTORY pExportDir, IN const char* pszName, OUT DWORD& dwVal, OUT char* pszDataType)
{
    if (strcmp(pszName, "Characteristics") == 0)
    {
        GetDataType(sizeof(pExportDir->Characteristics), pszDataType);
        dwVal = (DWORD)pExportDir->Characteristics;
    }
    else if (strcmp(pszName, "TimeDateStamp") == 0)
    {
        GetDataType(sizeof(pExportDir->TimeDateStamp), pszDataType);
        dwVal = pExportDir->TimeDateStamp;
    }
    else if (strcmp(pszName, "MajorVersion") == 0)
    {
        GetDataType(sizeof(pExportDir->MajorVersion), pszDataType);
        dwVal = pExportDir->MajorVersion;
    }
    else if (strcmp(pszName, "MinorVersion") == 0)
    {
        GetDataType(sizeof(pExportDir->MinorVersion), pszDataType);
        dwVal = pExportDir->MinorVersion;
    }
    else if (strcmp(pszName, "Name") == 0)
    {
        GetDataType(sizeof(pExportDir->Name), pszDataType);
        dwVal = pExportDir->Name;
    }
    else if (strcmp(pszName, "Base") == 0)
    {
        GetDataType(sizeof(pExportDir->Base), pszDataType);
        dwVal = pExportDir->Base;
    }
    else if (strcmp(pszName, "NumberOfFunctions") == 0)
    {
        GetDataType(sizeof(pExportDir->NumberOfFunctions), pszDataType);
        dwVal = pExportDir->NumberOfFunctions;
    }
    else if (strcmp(pszName, "NumberOfNames") == 0)
    {
        GetDataType(sizeof(pExportDir->NumberOfNames), pszDataType);
        dwVal = pExportDir->NumberOfNames;
    }
    else if (strcmp(pszName, "AddressOfFunctions") == 0)
    {
        GetDataType(sizeof(pExportDir->AddressOfFunctions), pszDataType);
        dwVal = pExportDir->AddressOfFunctions;
    }
    else if (strcmp(pszName, "AddressOfNames") == 0)
    {
        GetDataType(sizeof(pExportDir->AddressOfNames), pszDataType);
        dwVal = pExportDir->AddressOfNames;
    }
    else if (strcmp(pszName, "AddressOfNameOrdinals") == 0)
    {
        GetDataType(sizeof(pExportDir->AddressOfNameOrdinals), pszDataType);
        dwVal = pExportDir->AddressOfNameOrdinals;
    }
}

void MainWindow::AddExportDirFunc(IN PIMAGE_EXPORT_DIRECTORY pExportDir)
{
    DWORD hModule = (DWORD)m_hImageBase;
    //导出表三个数组的关系
    //AddressOfNames和AddressOfNameOrdinals是一 一对应关系（1 一对应）
    //AddressOfNameOrdinals指向AddressOfFunctions
    //索引值=编号-nBase，利用索引值检索查找
    //名称导出，检索顺序AddressOfNames-》AddressOfNameOrdinals-》AddressOfFunctions
    //序号导出，检索顺序AddressOfNames-》[AddressOfNameOrdinals]-》AddressOfFunctions

    //起始编号
    DWORD dwBase = pExportDir->Base;
    //导出表函数总数项
    DWORD dwNumOfFuncs = pExportDir->NumberOfFunctions;
    //导出表按名称导出数量
    DWORD dwNumOfNames = pExportDir->NumberOfNames;
    //函数地址表 DWORD
    DWORD dwAryFunctionsFOA = RVAToOffset(pExportDir->AddressOfFunctions, (PVOID)hModule);
    PDWORD pdwAryFunctions = (PDWORD)(dwAryFunctionsFOA + hModule);
    //名称地址表 DWORD
    DWORD dwAryNamesFOA = RVAToOffset(pExportDir->AddressOfNames, (PVOID)hModule);
    PDWORD pdwAryNames = (PDWORD)(dwAryNamesFOA + hModule);
    //序号地址表 WORD
    DWORD dwAryOrdinalsFOA = RVAToOffset(pExportDir->AddressOfNameOrdinals, (PVOID)hModule);
    PWORD pwAryOrdinals = (PWORD)(dwAryOrdinalsFOA + hModule);
    //解析导出表函数
    for (DWORD i = 0; i < dwNumOfFuncs; i++)
    {
        //过滤无效地址
        if (pdwAryFunctions[i] == 0)
            continue;

        size_t stBeginRow = 2;
        int j = 0;
        //检索名字(同时检索序号），有结果则是名称导出
        for (; j < dwNumOfNames; j++) //名字和序号一一对应
        {
            if (pwAryOrdinals[j] == i) //序号地址表指向函数地址表中的索引值，也就是说在函数地址表找到了函数
            {
                
                EXPORT_FUNC tagExportFunc;
                memset(&tagExportFunc, 0, sizeof(EXPORT_FUNC));
                                            //RVAToOffset(pdwAryNames[j], (PVOID)hModule) --> FOA
                                            //
                char* pszFuncName = (char*)(RVAToOffset(pdwAryNames[j], (PVOID)hModule) + hModule);
                tagExportFunc.m_Ordinal = dwBase + pwAryOrdinals[j];
                tagExportFunc.m_FuncRVA = pdwAryFunctions[i];
                tagExportFunc.m_NameOrdinal = pwAryOrdinals[j];
                tagExportFunc.m_NameRVA = pdwAryNames[j];
                tagExportFunc.m_szName = pszFuncName;

                AddExportFuncItem(&tagExportFunc, stBeginRow);
                ++stBeginRow;

                //存储文件偏移,尾部添加
                //DWORD dwOffsetFOA = 0;

                //dwOffsetFOA = dwAryFunctionsFOA + i*sizeof(DWORD);
                //m_vctExportOffset.push_back(dwOffsetFOA);

                //dwOffsetFOA = dwAryOrdinalsFOA + j*sizeof(WORD);
                //m_vctExportOffset.push_back(dwOffsetFOA);

                //dwOffsetFOA = dwAryNamesFOA + j * sizeof(DWORD);
                //m_vctExportOffset.push_back(dwOffsetFOA);

                //dwOffsetFOA = RVAToOffset(pdwAryNames[j], (PVOID)hModule);
                //m_vctExportOffset.push_back(dwOffsetFOA);

                //存储偏移，头部添加
                DWORD dwOffsetFOA = 0;

                dwOffsetFOA = dwAryFunctionsFOA + i*sizeof(DWORD);
                m_vctExportOffset.insert(m_vctExportOffset.begin(), dwOffsetFOA);

                dwOffsetFOA = dwAryOrdinalsFOA + j*sizeof(WORD);
                m_vctExportOffset.insert(m_vctExportOffset.begin(), dwOffsetFOA);

                dwOffsetFOA = dwAryNamesFOA + j * sizeof(DWORD);
                m_vctExportOffset.insert(m_vctExportOffset.begin(), dwOffsetFOA);

                dwOffsetFOA = RVAToOffset(pdwAryNames[j], (PVOID)hModule);
                m_vctExportOffset.insert(m_vctExportOffset.begin(), dwOffsetFOA);
                //名称导出显示
                //qDebug() << "nFunctions " << dwBase + pwAryOrdinals[j] << '\n'
                //    << "Function RVA " << pdwAryFunctions[i] << '\n'
                //    << "Name Ordinal " << pwAryOrdinals[j] << '\n'
                //    << "Name RVA " << pdwAryNames[j] << '\n'
                //    << "Name " << pszFuncName << '\n';
                break;
            }
        }

        //虚序号导出
        if (j == dwNumOfNames)
        {
            EXPORT_FUNC tagExportFunc;
            memset(&tagExportFunc, 0, sizeof(EXPORT_FUNC));
            //虚序号导出显示
            tagExportFunc.m_Ordinal = dwBase + i;
            tagExportFunc.m_FuncRVA = pdwAryFunctions[i];
            tagExportFunc.m_NameOrdinal = USHRT_MAX;
            tagExportFunc.m_NameRVA = ULONG_MAX;
            tagExportFunc.m_szName = NULL;

            AddExportFuncItem(&tagExportFunc, stBeginRow);
            ++stBeginRow;


            //存储文件偏移，尾部添加
            //DWORD dwOffsetFOA = 0;
            //dwOffsetFOA = dwAryFunctionsFOA + i * sizeof(DWORD);
            //m_vctExportOffset.push_back(dwOffsetFOA);
            //m_vctExportOffset.push_back(-1);
            //m_vctExportOffset.push_back(-1);
            //m_vctExportOffset.push_back(-1);

            //存储文件偏移，头部添加
            DWORD dwOffsetFOA = 0;
            dwOffsetFOA = dwAryFunctionsFOA + i * sizeof(DWORD);
            m_vctExportOffset.insert(m_vctExportOffset.begin(), dwOffsetFOA);
            m_vctExportOffset.insert(m_vctExportOffset.begin(), -1);
            m_vctExportOffset.insert(m_vctExportOffset.begin(), -1);
            m_vctExportOffset.insert(m_vctExportOffset.begin(), -1);

            //qDebug() << "nFunctions " << dwBase + i << '\n'
            //    << "Function RVA " << pdwAryFunctions[i] << '\n'
            //    << "Name Ordinal " << "N/A" << '\n'
            //    << "Name RVA " << "N/A" << '\n'
            //    << "Name " << "N/A" << '\n';
        }
    }

    //-----------------------
    //Ordinal, Function RVA, Name Ordinal, Name RVA, Name
}

void MainWindow::AddExportFuncItem(PEXPORT_FUNC p, size_t nRow)
{
    ui->tw_export_dir_2->insertRow(nRow);

    QString qstr;
    QTableWidgetItem* pQTWItem = NULL;

    qstr = QString("%1").arg(p->m_Ordinal, 8, 16, QLatin1Char('0'));
    pQTWItem = new QTableWidgetItem(qstr.toUpper());
    ui->tw_export_dir_2->setItem(nRow, 0, pQTWItem);

    qstr = QString("%1").arg(p->m_FuncRVA, 8, 16, QLatin1Char('0'));
    pQTWItem = new QTableWidgetItem(qstr.toUpper());
    ui->tw_export_dir_2->setItem(nRow, 1, pQTWItem);

    qDebug() << p->m_NameOrdinal << '\n';
    if (p->m_NameOrdinal != USHRT_MAX)
    {
        qstr = QString("%1").arg(p->m_NameOrdinal, 4, 16, QLatin1Char('0'));
    }
    else {
        qstr = QString(tr("N/A"));
    }
    pQTWItem = new QTableWidgetItem(qstr.toUpper());
    ui->tw_export_dir_2->setItem(nRow, 2, pQTWItem);

    if (p->m_NameRVA != ULONG_MAX)
    {
        qstr = QString("%1").arg(p->m_NameRVA, 8, 16, QLatin1Char('0'));
    }
    else {
        qstr = QString(tr("N/A"));
    }
    pQTWItem = new QTableWidgetItem(qstr.toUpper());
    ui->tw_export_dir_2->setItem(nRow, 3, pQTWItem);

    if (p->m_szName)
    {
        qstr = QString("%1").arg(p->m_szName);
    }
    else
    {
        qstr = QString(tr("N/A"));
    }
    pQTWItem = new QTableWidgetItem(qstr);
    ui->tw_export_dir_2->setItem(nRow, 4, pQTWItem);
}

void MainWindow::ShowTlsDirectory()
{
    InitTlsDir();

    //定位到TLS表
    PIMAGE_DOS_HEADER pDosHeader = NULL;
    PIMAGE_NT_HEADERS pNTHeader = NULL;
    PIMAGE_OPTIONAL_HEADER pOptHeader = NULL;
    PIMAGE_EXPORT_DIRECTORY pTlsDir = NULL;
    DWORD dwTlsDirRVA = 0;
    DWORD dwTlsDirFOA = 0;

    pDosHeader = (PIMAGE_DOS_HEADER)m_hImageBase;
    pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
    pOptHeader = &pNTHeader->OptionalHeader;
    dwTlsDirRVA = pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress;
    dwTlsDirFOA = RVAToOffset(dwTlsDirRVA, m_hImageBase);
    pTlsDir = (PIMAGE_EXPORT_DIRECTORY)((DWORD)m_hImageBase + dwTlsDirFOA);

    int nBaseOffset = dwTlsDirFOA;
    int nRow = 0;
    //int nOffset, char *pszName, int n = 4
    //PIMAGE_EXPORT_DIRECTORY pExportDir = (PIMAGE_EXPORT_DIRECTORY)p;
    //pExportDir->AddressOfNameOrdinals
    //IMAGE_TLS_DIRECTORY
    /*
    typedef struct _IMAGE_TLS_DIRECTORY32 {
    DWORD   StartAddressOfRawData;
    DWORD   EndAddressOfRawData;
    DWORD   AddressOfIndex;             // PDWORD
    DWORD   AddressOfCallBacks;         // PIMAGE_TLS_CALLBACK *
    DWORD   SizeOfZeroFill;
    union {
        DWORD Characteristics;
        struct {
            DWORD Reserved0 : 20;
            DWORD Alignment : 4;
            DWORD Reserved1 : 8;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;

    } IMAGE_TLS_DIRECTORY32;
    typedef IMAGE_TLS_DIRECTORY32 * PIMAGE_TLS_DIRECTORY32;
    */
    AddInfoEx(nBaseOffset, nRow, "StartAddressOfRawData", pTlsDir, "IMAGE_TLS_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "EndAddressOfRawData", pTlsDir, "IMAGE_TLS_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "AddressOfIndex", pTlsDir, "IMAGE_TLS_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "AddressOfCallBacks", pTlsDir, "IMAGE_TLS_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "SizeOfZeroFill", pTlsDir, "IMAGE_TLS_DIRECTORY");
    AddInfoEx(nBaseOffset, nRow, "Characteristics", pTlsDir, "IMAGE_TLS_DIRECTORY");
}

void MainWindow::InitTlsDir()
{
    //设置页面
    ui->stackedWidget->setCurrentWidget(ui->page_tls_dir);
    //清理
    ui->tw_petls->setRowCount(0);
    ui->tw_petls->clearContents();
}

void MainWindow::FormatTlsDirMember(IN PIMAGE_TLS_DIRECTORY pTlsDir, IN const char* pszName, OUT DWORD& dwVal, OUT char* pszDataType)
{
    if (strcmp(pszName, "StartAddressOfRawData") == 0)
    {
        GetDataType(sizeof(pTlsDir->StartAddressOfRawData), pszDataType);
        dwVal = (DWORD)pTlsDir->StartAddressOfRawData;
    }
    else if (strcmp(pszName, "EndAddressOfRawData") == 0)
    {
        GetDataType(sizeof(pTlsDir->EndAddressOfRawData), pszDataType);
        dwVal = pTlsDir->EndAddressOfRawData;
    }
    else if (strcmp(pszName, "AddressOfIndex") == 0)
    {
        GetDataType(sizeof(pTlsDir->AddressOfIndex), pszDataType);
        dwVal = pTlsDir->AddressOfIndex;
    }
    else if (strcmp(pszName, "AddressOfCallBacks") == 0)
    {
        GetDataType(sizeof(pTlsDir->AddressOfCallBacks), pszDataType);
        dwVal = pTlsDir->AddressOfCallBacks;
    }
    else if (strcmp(pszName, "SizeOfZeroFill") == 0)
    {
        GetDataType(sizeof(pTlsDir->SizeOfZeroFill), pszDataType);
        dwVal = pTlsDir->SizeOfZeroFill;
    }
    else if (strcmp(pszName, "Characteristics") == 0)
    {
        GetDataType(sizeof(pTlsDir->Characteristics), pszDataType);
        dwVal = pTlsDir->Characteristics;
    }
}

void MainWindow::ShowAddressConverterInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->page_AC);
}

void MainWindow::AddSectionOffsetSlot(const QModelIndex)
{
    qDebug() << "AddSectionOffsetSlot(const QModelIndex)" << 'n';
    int nBaseOffset = 0;  //第一个节相对于DOS头在文件中偏移
    int nSize = 40;  //一个节2行半，40个字节

    GetFirstSectionHeader();
    nBaseOffset = (int)m_pSH - (int)m_pDH;

    //根据选中行，调整基础偏移
    int nRow = ui->tw_pesh_1->currentRow();
    nBaseOffset += nRow * nSize;

    //添加偏移数据到表格显示
    QString qstrCol;
    QTableWidgetItem *pColumn;
    IMAGE_SECTION_HEADER tagSh;

    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 0, pColumn);
    nBaseOffset += sizeof(tagSh.Name);
    
    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 1, pColumn);
    nBaseOffset += sizeof(tagSh.Misc.VirtualSize);

    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 2, pColumn);
    nBaseOffset += sizeof(tagSh.VirtualAddress);

    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 3, pColumn);
    nBaseOffset += sizeof(tagSh.SizeOfRawData);

    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 4, pColumn);
    nBaseOffset += sizeof(tagSh.PointerToRawData);

    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 5, pColumn);
    nBaseOffset += sizeof(tagSh.PointerToRelocations);

    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 6, pColumn);
    nBaseOffset += sizeof(tagSh.PointerToLinenumbers);

    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 7, pColumn);
    nBaseOffset += sizeof(tagSh.NumberOfRelocations);

    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 8, pColumn);
    nBaseOffset += sizeof(tagSh.NumberOfRelocations);

    qstrCol = QString("%1").arg(nBaseOffset, 8, 16, QLatin1Char('0'));
    pColumn = new QTableWidgetItem(qstrCol.toUpper());
    ui->tw_pesh_2->setItem(0, 9, pColumn);
}

void MainWindow::VA2RVA2FASlot()
{
    if (!m_hImageBase)
        return;

    QString qstrHexVA, qstrHexRVA, qstrHexFA;

    qstrHexVA = ui->lineEdit_VA->text();
    qstrHexRVA = ui->lineEdit_RVA->text();
    qstrHexFA = ui->lineEdit_FA->text();

    if (!qstrHexVA.isEmpty()) //VA
    {
        //显示VA
        bool ok;
        int hexnumVA = qstrHexVA.toInt(&ok, 16); // 表示以16进制方式读取字符串
        QString qstrTmpVA = QString("%1").arg(hexnumVA, 8, 16, QLatin1Char('0'));
        ui->lineEdit_VA->setText(qstrTmpVA.toUpper());

        //显示RVA
        PIMAGE_DOS_HEADER pDH = NULL;
        PIMAGE_NT_HEADERS pNTH = NULL;
        PIMAGE_OPTIONAL_HEADER pOH = NULL;
        pDH = (PIMAGE_DOS_HEADER)m_hImageBase;
        pNTH = (PIMAGE_NT_HEADERS)((DWORD)pDH + pDH->e_lfanew);
        pOH = &pNTH->OptionalHeader;

        int hexnumRVA = hexnumVA - pOH->ImageBase;
        QString qstrTmpRVA = QString("%1").arg(hexnumRVA, 8, 16, QLatin1Char('0'));
        ui->lineEdit_RVA->setText(qstrTmpRVA.toUpper());

        //显示FA
        int hexnumFA = RVAToOffset(hexnumRVA, m_hImageBase);
        QString qstrTmpFA;
        if (hexnumFA < 0)
        {
            hexnumFA = 0;
            qstrTmpFA = QString("%1").arg(0, 8, 16, QLatin1Char('0'));
        }
        else
        {
            qstrTmpFA = QString("%1").arg(hexnumFA, 8, 16, QLatin1Char('0'));
        }      
        ui->lineEdit_FA->setText(qstrTmpFA.toUpper());
    }
}

void MainWindow::RVA2FA2VASlot()
{
    if (!m_hImageBase)
        return;

    QString qstrHexVA, qstrHexRVA, qstrHexFA;

    qstrHexVA = ui->lineEdit_VA->text();
    qstrHexRVA = ui->lineEdit_RVA->text();
    qstrHexFA = ui->lineEdit_FA->text();

    if (!qstrHexRVA.isEmpty()) //VA
    {
        //显示RVA
        bool ok;
        int hexnumRVA = qstrHexRVA.toInt(&ok, 16); // 表示以16进制方式读取字符串
        QString qstrTmpRVA = QString("%1").arg(hexnumRVA, 8, 16, QLatin1Char('0'));
        ui->lineEdit_RVA->setText(qstrTmpRVA.toUpper());

        //显示FA
        int hexnumFA = RVAToOffset(hexnumRVA, m_hImageBase);
        QString qstrTmpFA;
        if (hexnumFA < 0)
        {
            hexnumFA = 0;
            qstrTmpFA = QString("%1").arg(0, 8, 16, QLatin1Char('0'));
        }
        else
        {
            qstrTmpFA = QString("%1").arg(hexnumFA, 8, 16, QLatin1Char('0'));
        }
        ui->lineEdit_FA->setText(qstrTmpFA.toUpper());

        //显示VA
        int hexnumVA = 0;
        if (hexnumFA)
        {
            hexnumVA = Offset2VA(hexnumFA, m_hImageBase);
        }
        else
        {
            hexnumVA = Offset2VA(hexnumRVA, m_hImageBase);
        }
        QString qstrTmpVA = QString("%1").arg(hexnumVA, 8, 16, QLatin1Char('0'));
        ui->lineEdit_VA->setText(qstrTmpVA.toUpper());
    }
}

void MainWindow::FA2VA2RVASlot()
{
    if (!m_hImageBase)
        return;

    QString qstrHexVA, qstrHexRVA, qstrHexFA;

    qstrHexVA = ui->lineEdit_VA->text();
    qstrHexRVA = ui->lineEdit_RVA->text();
    qstrHexFA = ui->lineEdit_FA->text();

    if (!qstrHexFA.isEmpty())
    {
        //显示FA
        bool ok;
        int hexnumFA = qstrHexFA.toInt(&ok, 16); // 表示以16进制方式读取字符串
        QString qstrTmpFA;
        if (hexnumFA < 0)
        {
            hexnumFA = 0;
            qstrTmpFA = QString("%1").arg(0, 8, 16, QLatin1Char('0'));
        }
        else
        {
            qstrTmpFA = QString("%1").arg(hexnumFA, 8, 16, QLatin1Char('0'));
        }
        ui->lineEdit_FA->setText(qstrTmpFA.toUpper());

        //显示VA
        int hexnumVA = Offset2VA(hexnumFA, m_hImageBase);
        QString qstrTmpVA = QString("%1").arg(hexnumVA, 8, 16, QLatin1Char('0'));
        ui->lineEdit_VA->setText(qstrTmpVA.toUpper());

        //显示RVA
        PIMAGE_DOS_HEADER pDH = NULL;
        PIMAGE_NT_HEADERS pNTH = NULL;
        PIMAGE_OPTIONAL_HEADER pOH = NULL;
        pDH = (PIMAGE_DOS_HEADER)m_hImageBase;
        pNTH = (PIMAGE_NT_HEADERS)((DWORD)pDH + pDH->e_lfanew);
        pOH = &pNTH->OptionalHeader;

        int hexnumRVA = hexnumVA - pOH->ImageBase;
        QString qstrTmpRVA = QString("%1").arg(hexnumRVA, 8, 16, QLatin1Char('0'));
        ui->lineEdit_RVA->setText(qstrTmpRVA.toUpper());
    }
}

void MainWindow::AddExportFuncOffsetSlot(const QModelIndex)
{
    qDebug() << m_vctExportOffset.size() << '\n';
    //遍历vector,导出表函数偏移计算4个元素，
    //行数i*4, 终止条件n<i*4+4
    /*
    比如：
    第0行，起始下标0*4, 结束下标0*4+4
    第1行，起始下村1*4，结束下标1*4+4
    */
    size_t stBeginRow = 2;
    size_t stCurRow = ui->tw_export_dir_2->currentRow();
    size_t stOffsetBeginRow = 0;

    if (stCurRow < stBeginRow)
        return;

    stOffsetBeginRow = stCurRow - stBeginRow; //i
    int i = stOffsetBeginRow*4; //i*4
    int n = stOffsetBeginRow * 4 + 4; //i*4+4
    QString qstr;
    QTableWidgetItem* pQTWItem = NULL;

    qstr = QString(tr("N/A"));
    pQTWItem = new QTableWidgetItem(qstr.toUpper());
    ui->tw_export_dir_2->setItem(0, 0, pQTWItem);

    int nColumn = 1;
    //for (;i<n;i++)
    //{
    //    if (m_vctExportOffset[i])
    //    {
    //        qstr = QString("%1").arg(m_vctExportOffset[i], 8, 16, QLatin1Char('0'));
    //    }
    //    else {
    //        qstr = QString(tr("N/A"));
    //    }

    //    pQTWItem = new QTableWidgetItem(qstr.toUpper());
    //    ui->tw_export_dir_2->setItem(0, nColumn, pQTWItem);
    //    nColumn++;
    //}

    for (; n>i; n--) //n 4 --> [3], i 0
    {
        if (m_vctExportOffset[n-1] != -1)
        {
            qstr = QString("%1").arg(m_vctExportOffset[n-1], 8, 16, QLatin1Char('0'));
        }
        else {
            qstr = QString(tr("N/A"));
        }

        pQTWItem = new QTableWidgetItem(qstr.toUpper());
        ui->tw_export_dir_2->setItem(0, nColumn, pQTWItem);
        nColumn++;
    }
}

void MainWindow::ShowImpDirOffsetAndFuncSlot(const QModelIndex)
{
    ui->tw_peid_2->setRowCount(0);
    ui->tw_peid_2->clearContents();
    ui->tw_peid_2->insertRow(0);
    ui->tw_peid_2->insertRow(1);

    //取得入口地址所需参数
    PIMAGE_DOS_HEADER pDH = NULL;
    PIMAGE_NT_HEADERS pNTH = NULL;
    PIMAGE_OPTIONAL_HEADER pOH = NULL;
    PIMAGE_IMPORT_DESCRIPTOR pImportDir = NULL;
    DWORD dwImportDirRVA = 0;
    DWORD dwImportDirFOA = 0;

    pDH = (PIMAGE_DOS_HEADER)m_hImageBase;
    pNTH = (PIMAGE_NT_HEADERS)((DWORD)pDH + pDH->e_lfanew);
    pOH = &pNTH->OptionalHeader;
    //导入表RVA
    dwImportDirRVA = pOH->DataDirectory[1].VirtualAddress;

    //RVA转换得到FOA(导入表数据)
    dwImportDirFOA = RVAToOffset(dwImportDirRVA, m_hImageBase);
    //得到内存中导入表地址
    pImportDir = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)m_hImageBase + dwImportDirFOA);

    //获取点击行DLL模块名称
    size_t stRow = ui->tw_peid_1->currentRow();
    QString qstrDllName = ui->tw_peid_1->item(stRow, 0)->text();

    //遍历导入表数据
    while (pImportDir->FirstThunk)
    {
        DWORD dwDllNameFOA;
        dwDllNameFOA = RVAToOffset(pImportDir->Name, m_hImageBase);
        char szDllName[256] = { 0 };
        char* pszDllName = NULL;
        pszDllName = (char*)((DWORD)m_hImageBase + dwDllNameFOA);
        strcpy(szDllName, pszDllName);

        QString qstrCol;
        //char* 转 QString
        qstrCol = QString(szDllName);

        if (qstrCol.compare(qstrDllName) == 0)
        {
            //上面的表格
            DWORD dwOffsetFOA = 0;
            QString qstrOffsetFOA;
            QTableWidgetItem *item = NULL;
            //显示在文件中的偏移FOA
            dwOffsetFOA = RVAToOffset(pImportDir->Name, m_hImageBase);

            //Module Name
            qstrOffsetFOA = QString("%1").arg(dwOffsetFOA, 8, 16, QLatin1Char('0'));
            item = new QTableWidgetItem(qstrOffsetFOA.toUpper());
            ui->tw_peid_1->setItem(0, 0, item);

            //Imports
            //N/A
            qstrOffsetFOA = QString(tr("N/A"));
            item = new QTableWidgetItem(qstrOffsetFOA.toUpper());
            ui->tw_peid_1->setItem(0, 1, item);

            //OFTs
            qstrOffsetFOA = QString("%1").arg(dwImportDirFOA, 8, 16, QLatin1Char('0'));
            item = new QTableWidgetItem(qstrOffsetFOA.toUpper());
            ui->tw_peid_1->setItem(0, 2, item);
            dwImportDirFOA += sizeof(pImportDir->OriginalFirstThunk);

            //TimeDateStamp
            qstrOffsetFOA = QString("%1").arg(dwImportDirFOA, 8, 16, QLatin1Char('0'));
            item = new QTableWidgetItem(qstrOffsetFOA.toUpper());
            ui->tw_peid_1->setItem(0, 3, item);
            dwImportDirFOA += sizeof(pImportDir->TimeDateStamp);

            //ForwarderChain
            qstrOffsetFOA = QString("%1").arg(dwImportDirFOA, 8, 16, QLatin1Char('0'));
            item = new QTableWidgetItem(qstrOffsetFOA.toUpper());
            ui->tw_peid_1->setItem(0, 4, item);
            dwImportDirFOA += sizeof(pImportDir->ForwarderChain);

            //Name RVA
            qstrOffsetFOA = QString("%1").arg(dwImportDirFOA, 8, 16, QLatin1Char('0'));
            item = new QTableWidgetItem(qstrOffsetFOA.toUpper());
            ui->tw_peid_1->setItem(0, 5, item);
            dwImportDirFOA += sizeof(pImportDir->Name);

            //FTs(IAT)
            qstrOffsetFOA = QString("%1").arg(dwImportDirFOA, 8, 16, QLatin1Char('0'));
            item = new QTableWidgetItem(qstrOffsetFOA.toUpper());
            ui->tw_peid_1->setItem(0, 6, item);

            //下面的表格
            //添加表头
            ui->tw_peid_2->setItem(1, 0, new QTableWidgetItem(tr("Dword")));
            ui->tw_peid_2->setItem(1, 1, new QTableWidgetItem(tr("Dword")));
            ui->tw_peid_2->setItem(1, 2, new QTableWidgetItem(tr("Word")));
            ui->tw_peid_2->setItem(1, 3, new QTableWidgetItem(tr("szAnsi")));

            //OFTs(INT), FTs(IAT), Hint, Name
            
            DWORD dwOFTsFOA = 0;
            DWORD dwFTsFOA = 0; 
            DWORD* pdwFTsFOA = NULL;

            dwFTsFOA = RVAToOffset(pImportDir->FirstThunk, m_hImageBase);
            pdwFTsFOA = (DWORD*)((DWORD)m_hImageBase + dwFTsFOA);
            //遍历IAT，获取导入函数个数
            size_t stRow = 2;
            while (*pdwFTsFOA)
            {
                DWORD dwOFTs = 0;
                DWORD dwFTs = 0;
                WORD nwHint = 0;
                char szFuncName[256] = { 0 };
                QString qstr;
                QTableWidgetItem* item = NULL;

                ui->tw_peid_2->insertRow(stRow);

                //OFTs
                dwOFTs = *pdwFTsFOA;
                qstr = QString("%1").arg(dwOFTs, 8, 16, QLatin1Char('0'));
                item = new QTableWidgetItem(qstr.toUpper());
                ui->tw_peid_2->setItem(stRow, 0, item);

                //FTs
                dwFTs = *pdwFTsFOA;
                qstr = QString("%1").arg(dwFTs, 8, 16, QLatin1Char('0'));
                item = new QTableWidgetItem(qstr.toUpper());
                ui->tw_peid_2->setItem(stRow, 1, item);

                //IMAGE_IMPORT_BY_NAME
                PIMAGE_IMPORT_BY_NAME pImpByName = NULL;
                DWORD dwImpByName = 0;

                dwImpByName = RVAToOffset(dwFTs, m_hImageBase);
                pImpByName = (PIMAGE_IMPORT_BY_NAME)((DWORD)m_hImageBase + dwImpByName);
                //Hint
                qstr = QString("%1").arg(pImpByName->Hint, 4, 16, QLatin1Char('0'));
                item = new QTableWidgetItem(qstr.toUpper());
                ui->tw_peid_2->setItem(stRow, 2, item);

                //Name
                qstr = QString(pImpByName->Name);
                item = new QTableWidgetItem(qstr);
                ui->tw_peid_2->setItem(stRow, 3, item);

                ++pdwFTsFOA;
            }

            break;
        }
        ++pImportDir;
    }
}

void MainWindow::ShowImpDirFuncOffsetSlot(const QModelIndex)
{
    //取得入口地址所需参数
    PIMAGE_DOS_HEADER pDH = NULL;
    PIMAGE_NT_HEADERS pNTH = NULL;
    PIMAGE_OPTIONAL_HEADER pOH = NULL;
    PIMAGE_IMPORT_DESCRIPTOR pImportDir = NULL;
    DWORD dwImportDirRVA = 0;
    DWORD dwImportDirFOA = 0;

    pDH = (PIMAGE_DOS_HEADER)m_hImageBase;
    pNTH = (PIMAGE_NT_HEADERS)((DWORD)pDH + pDH->e_lfanew);
    pOH = &pNTH->OptionalHeader;
    //导入表RVA
    dwImportDirRVA = pOH->DataDirectory[1].VirtualAddress;

    //RVA转换得到FOA(导入表数据)
    dwImportDirFOA = RVAToOffset(dwImportDirRVA, m_hImageBase);
    //得到内存中导入表地址
    pImportDir = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)m_hImageBase + dwImportDirFOA);

    //获取点击行导入函数名称
    size_t stRow = ui->tw_peid_2->currentRow();
    QString qstrFuncName = ui->tw_peid_2->item(stRow, 3)->text();

    BOOL bStopFlag = FALSE; //退出标志

    //遍历导入表数据
    while (pImportDir->FirstThunk)
    {
        DWORD dwFTsFOA = 0;
        DWORD* pdwThunkData = NULL;

        dwFTsFOA = RVAToOffset(pImportDir->FirstThunk, m_hImageBase);
        pdwThunkData = (DWORD*)((DWORD)m_hImageBase + dwFTsFOA);

        // 记录当前模块内，IAT导入函数偏移
        DWORD dwCurDllFuncOffset = 0;

        //遍历IAT
        while (*pdwThunkData)
        {

            DWORD dwOFTsFOA = 0;
            QString qstr;
            QTableWidgetItem* item = NULL;
            size_t stRow = 0;

            //比较导入函数名
            DWORD dwThunkDataRVA = 0;
            dwThunkDataRVA = *pdwThunkData;

            DWORD dwImpByNameFOA = 0;
            PIMAGE_IMPORT_BY_NAME pImpByName = NULL;
            dwImpByNameFOA = RVAToOffset(dwThunkDataRVA, m_hImageBase);
            pImpByName = (PIMAGE_IMPORT_BY_NAME)((DWORD)m_hImageBase + dwImpByNameFOA);

            if (qstrFuncName.compare(pImpByName->Name) == 0)
            {
                //添加表头
                ui->tw_peid_2->setItem(1, 0, new QTableWidgetItem(tr("Dword")));
                ui->tw_peid_2->setItem(1, 1, new QTableWidgetItem(tr("Dword")));
                ui->tw_peid_2->setItem(1, 2, new QTableWidgetItem(tr("Word")));
                ui->tw_peid_2->setItem(1, 3, new QTableWidgetItem(tr("szAnsi")));

                //OFTs(INT), FTs(IAT), Hint, Name

                DWORD dwOFTsFOA = 0;
                DWORD dwFTsFOA = 0;
                QString qstr;
                QTableWidgetItem* item = NULL;
                size_t stRow = 0;

                //OFTs
                dwOFTsFOA = RVAToOffset(pImportDir->OriginalFirstThunk + dwCurDllFuncOffset, m_hImageBase);
                qstr = QString("%1").arg(dwOFTsFOA, 8, 16, QLatin1Char('0'));
                item = new QTableWidgetItem(qstr.toUpper());
                ui->tw_peid_2->setItem(stRow, 0, item);

                //FTs(IAT)
                dwFTsFOA = RVAToOffset(pImportDir->FirstThunk + dwCurDllFuncOffset, m_hImageBase);
                qstr = QString("%1").arg(dwFTsFOA, 8, 16, QLatin1Char('0'));
                item = new QTableWidgetItem(qstr.toUpper());
                ui->tw_peid_2->setItem(stRow, 1, item);

                //IMAGE_THUNK_DATA
                //Hint
                DWORD dwThunkDataRVA = 0;
                DWORD* pdwThunkData = NULL;
                pdwThunkData = (DWORD*)((DWORD)m_hImageBase + dwFTsFOA);
                dwThunkDataRVA = *pdwThunkData;

                //IMAGE_IMPORT_BY_NAME
                //RVA 转 FOA
                DWORD dwImpByNameFOA = 0;
                dwImpByNameFOA = RVAToOffset(dwThunkDataRVA, m_hImageBase);
                qstr = QString("%1").arg(dwImpByNameFOA, 8, 16, QLatin1Char('0'));
                item = new QTableWidgetItem(qstr.toUpper());
                ui->tw_peid_2->setItem(stRow, 2, item);
                dwImpByNameFOA += sizeof(WORD);

                //Name
                qstr = QString("%1").arg(dwImpByNameFOA, 8, 16, QLatin1Char('0'));
                item = new QTableWidgetItem(qstr.toUpper());
                ui->tw_peid_2->setItem(stRow, 3, item);

                bStopFlag = TRUE;

                break;
            }

            if (bStopFlag)
            {
                break;
            }

            dwCurDllFuncOffset += sizeof(pdwThunkData);

            ++pdwThunkData;
        }

        if (bStopFlag)
        {
            break;
        }

        ++pImportDir;
    }
}

void MainWindow::AddRelocationOffsetSlot(const QModelIndex QMIndex)
{
    size_t stCurRow = QMIndex.row();
    size_t stOffsetRow = 0;
    size_t stBeginRow = 2;
    DWORD dwRVA_FOA = 0;
    QString qstrOffset;
    QTableWidgetItem *item = NULL;

    if (stCurRow < stBeginRow)
        return;

    //定位到重定位表
    PIMAGE_DOS_HEADER pDosHeader = NULL;
    PIMAGE_NT_HEADERS pNTHeader = NULL;
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;
    PIMAGE_BASE_RELOCATION pRelocationDir = NULL;
    DWORD dwRelocationDirRVA = 0;
    DWORD dwRelocationDirFOA = 0;

    pDosHeader = (PIMAGE_DOS_HEADER)m_hImageBase;
    pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
    pOptionalHeader = &pNTHeader->OptionalHeader;

    dwRelocationDirRVA = pOptionalHeader->DataDirectory[5].VirtualAddress;
    //首页重定位页FOA
    dwRelocationDirFOA = RVAToOffset(dwRelocationDirRVA, m_hImageBase);  //A00
    pRelocationDir = (PIMAGE_BASE_RELOCATION)((DWORD)dwRelocationDirFOA + (DWORD)m_hImageBase);

    bool ok;
    DWORD dwCurLineVirtualAddress = 0;
    QString qstrHexRVA = 0;
    qstrHexRVA = ui->tw_perd_1->item(stCurRow, 0)->text();
    dwCurLineVirtualAddress = qstrHexRVA.toInt(&ok, 16); // 表示以16进制方式读取字符串

    DWORD dwCurPageRelocRVA = 0;
    DWORD dwCurPageRelocFOA = 0;
    DWORD dwSizeOfBlock_FOA = 0;
    dwCurPageRelocFOA = dwRelocationDirFOA;
    while (dwCurPageRelocRVA = pRelocationDir->VirtualAddress)
    {
        if(dwCurPageRelocRVA != dwCurLineVirtualAddress)
        { 
            DWORD dwSizeOfBlock = pRelocationDir->SizeOfBlock;
            //计算后面重定位页
            pRelocationDir = (PIMAGE_BASE_RELOCATION)((BYTE*)pRelocationDir + dwSizeOfBlock);
            dwCurPageRelocFOA += dwSizeOfBlock; //包括IMAGE_BASE_RELOCATION
            continue;
        }
        break;
    }

    //偏移计算
    //重定位项：(目标页-8)/2，每荐2字节，4字节VirtualAddress,4字节SizeOfBlock
    //            RVA                                                             Size Of Block
    //第1行       首页FOA+sizeof(IMAGE_BASE_RELOCATION)+第0行重定位基数*2           RVA(前面的FOA)+sizeof(VirtualAddress)
    //第2行       首页FOA+sizeof(IMAGE_BASE_RELOCATION)+第1行重定位基数*2           RVA(前面的FOA)+sizeof(VirtualAddress)
    //...
    dwRVA_FOA = dwCurPageRelocFOA;
    dwSizeOfBlock_FOA = dwRVA_FOA + sizeof(pRelocationDir->VirtualAddress);
    //RVA
    qstrOffset = QString("%1").arg(dwRVA_FOA, 8, 16, QLatin1Char('0'));
    item = new QTableWidgetItem(qstrOffset.toUpper());
    ui->tw_perd_1->setItem(stOffsetRow, 0, item);
    //Size Of Block
    qstrOffset = QString("%1").arg(dwSizeOfBlock_FOA, 8, 16, QLatin1Char('0'));
    item = new QTableWidgetItem(qstrOffset.toUpper());
    ui->tw_perd_1->setItem(stOffsetRow, 1, item);
    //N/A
    qstrOffset = QString(tr("N/A"));
    item = new QTableWidgetItem(qstrOffset.toUpper());
    ui->tw_perd_1->setItem(stOffsetRow, 2, item);

    //遍历重定位项
    QueryRelocationItems();
}

void MainWindow::AddRelocItemOffsetSlot(const QModelIndex)
{
    //第N项偏移计算，从第1行计算
    //FOA + (N-stBeginRow)*2

    //取得当前页，重定位项RVA地址
    size_t stCurRow = ui->tw_perd_1->currentRow();
    size_t stBeginRow = 2;
    if (stCurRow < stBeginRow)
        return;

    //添加标题项
    ui->tw_perd_2->setItem(1, 0, new QTableWidgetItem(tr("Word")));
    ui->tw_perd_2->setItem(1, 1, new QTableWidgetItem(tr("N/A")));
    ui->tw_perd_2->setItem(1, 2, new QTableWidgetItem(tr("N/A")));

    //定位到重定位表
    PIMAGE_DOS_HEADER pDosHeader = NULL;
    PIMAGE_NT_HEADERS pNTHeader = NULL;
    PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;
    PIMAGE_BASE_RELOCATION pRelocationDir = NULL;
    DWORD dwRelocationDirRVA = 0;
    DWORD dwRelocationDirFOA = 0;

    pDosHeader = (PIMAGE_DOS_HEADER)m_hImageBase;
    pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
    pOptionalHeader = &pNTHeader->OptionalHeader;

    dwRelocationDirRVA = pOptionalHeader->DataDirectory[5].VirtualAddress;
    //首页重定位页FOA
    dwRelocationDirFOA = RVAToOffset(dwRelocationDirRVA, m_hImageBase);  //A00
    pRelocationDir = (PIMAGE_BASE_RELOCATION)((DWORD)dwRelocationDirFOA + (DWORD)m_hImageBase);

    bool ok;
    DWORD dwCurLineVirtualAddress = 0;
    QString qstrHexRVA = 0;
    qstrHexRVA = ui->tw_perd_1->item(stCurRow, 0)->text();
    dwCurLineVirtualAddress = qstrHexRVA.toInt(&ok, 16); // 表示以16进制方式读取字符串

    DWORD dwCurPageRelocRVA = 0;
    DWORD dwCurPageRelocFOA = 0;
    DWORD dwSizeOfBlock_FOA = 0;
    dwCurPageRelocFOA = dwRelocationDirFOA;
    while (dwCurPageRelocRVA = pRelocationDir->VirtualAddress)
    {
        if (dwCurPageRelocRVA != dwCurLineVirtualAddress)
        {
            DWORD dwSizeOfBlock = pRelocationDir->SizeOfBlock;
            //计算后面重定位页
            pRelocationDir = (PIMAGE_BASE_RELOCATION)((BYTE*)pRelocationDir + dwSizeOfBlock);
            dwCurPageRelocFOA += dwSizeOfBlock; //包括IMAGE_BASE_RELOCATION
            continue;
        }
        break;
    }

    //当前行的重定位项
    size_t stCurRowItem = ui->tw_perd_2->currentRow();
    DWORD dwCurRelocItemFOA = 0;
    dwCurRelocItemFOA = dwCurPageRelocFOA + sizeof(IMAGE_BASE_RELOCATION) + (stCurRowItem - stBeginRow) * 2;

    QString qstrOffset;
    QTableWidgetItem *item = NULL;
    qstrOffset = QString("%1").arg(dwCurRelocItemFOA, 8, 16, QLatin1Char('0'));
    item = new QTableWidgetItem(qstrOffset.toUpper());
    ui->tw_perd_2->setItem(0, 0, item);

    qstrOffset = QString(tr("N/A"));
    item = new QTableWidgetItem(qstrOffset.toUpper());
    ui->tw_perd_2->setItem(0, 1, item);

    qstrOffset = QString(tr("N/A"));
    item = new QTableWidgetItem(qstrOffset.toUpper());
    ui->tw_perd_2->setItem(0, 2, item);
}

void MainWindow::OnTreeViewClickSlot(const QModelIndex &index)
{
    //是否是PE文件
    if (!IsPE())
        return;

    //获取选中行
    QModelIndex SelIndex = ui->treeView->currentIndex();
    QString qstrSel = SelIndex.data().toString();

    //比较菜单项
    if (qstrSel.compare(TV_DOS_HEADER) == 0) //Dos Header
    {
        //ui->stackedWidget->setCurrentIndex(0);
        
        qDebug() << TV_DOS_HEADER << '\n';
        ShowDosHeaderInfo();
    }
    else if (qstrSel.compare(TV_NT_HEADERS) == 0) //Nt Header
    {
        qDebug() << TV_NT_HEADERS << '\n';
        ShowNtHeadersInfo();
    }
    else if (qstrSel.compare(TV_SECTION_HEADERS) == 0) //Section Headers [x]
    {
        qDebug() << TV_SECTION_HEADERS << '\n';
        ShowSectionDirectoryXInfo();
    }
    else if (qstrSel.compare(TV_EXPORT_DIRECTORY) == 0) //Export Directory
    {
        qDebug() << TV_EXPORT_DIRECTORY << '\n';
        if (!m_hImageBase)
            return;

        //查看
        ShowExportDirInfo();
    }
    else if (qstrSel.compare(TV_IMPORT_DIRECTORY) == 0)//Import Directory
    {
        qDebug() << TV_IMPORT_DIRECTORY << '\n';
        if (!m_hImageBase)
            return;

        //清理旧数据
        ui->tw_peid_1->setRowCount(0);
        ui->tw_peid_1->clearContents();
        ui->tw_peid_1->insertRow(0);
        ui->tw_peid_1->insertRow(1);

        ui->tw_peid_2->setRowCount(0);
        ui->tw_peid_2->clearContents();
        ui->tw_peid_2->insertRow(0);
        ui->tw_peid_2->insertRow(1);

        ShowImportDirectoryInfo();
    }
    else if (qstrSel.compare(TV_TLS_DIRECTORY) == 0)
    {
        qDebug() << TV_IMPORT_DIRECTORY << '\n';
        if (!m_hImageBase)
            return;

        ShowTlsDirectory();
    }
    else if (qstrSel.compare(TV_RELOCATION_DIRECTORY) == 0)//Relocation Directory
    {
        qDebug() << TV_RELOCATION_DIRECTORY << '\n';
        if (!m_hImageBase)
            return;

        //清理旧数据
        ui->tw_perd_1->setRowCount(0);
        ui->tw_perd_1->clearContents();
        ui->tw_perd_1->insertRow(0);
        ui->tw_perd_1->insertRow(1);

        ui->tw_perd_2->setRowCount(0);
        ui->tw_perd_2->clearContents();
        ui->tw_perd_2->insertRow(0);
        ui->tw_perd_2->insertRow(1);

        ShowRelocationDirectoryInfo();
    }
    else if (qstrSel.compare(TV_ADDRESS_CONVERTER) == 0) //Address Converter
    {
        qDebug() << TV_ADDRESS_CONVERTER << '\n';
        ShowAddressConverterInfo();
    }
    else if (qstrSel.compare(TV_NT_FH) == 0) //File Header
    {
        qDebug() << TV_NT_FH << '\n';
        ShowFileHeaderInfo();
    }
    else if (qstrSel.compare(TV_NT_OH) == 0) //Optional Header
    {
        ui->tableWidget->show();
        qDebug() << TV_NT_OH << '\n';
        ShowOptionalHeaderInfo();

    }
    else if (qstrSel.compare(TV_NT_OH_DD) == 0) //Data Directories [x]
    {
        qDebug() << TV_NT_OH_DD << '\n';
        ShowDataDirectoriesXInfo();

    }
    else
    {

    }
}
//
//void MainWindow::OpenFileSlot()
//{
//    QString fileName;
//    fileName = QFileDialog::getOpenFileName(this, tr("文件"), "", tr(text(*.txt)));
//
//    if (!fileName.isNull())
//    {
//        ui.lineEdit_open->setText(fileName);
//    }
//}
