#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Windows.h>

#include <vector>
using namespace std;

#include <QMainWindow>
#include <qstandarditemmodel.h>
#include <qstringlistmodel.h>
#include <qtablewidget.h>
#include <qtreeview.h>
#include <QModelIndex>
#include<QDragEnterEvent>
#include<QMimeData>
#include<QDropEvent>
#include<QUrl>

typedef struct tagExportFuncData
{
    DWORD m_Ordinal;
    DWORD m_FuncRVA;
    WORD m_NameOrdinal;
    DWORD m_NameRVA;
    char* m_szName;
} EXPORT_FUNC, *PEXPORT_FUNC;

class QStackedWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    //初始化
    void MyInitialize();
    //设置信号槽
    void SetConnect();
    //设置UI属性
    void SetUI();
    //初始化左侧树形结构(参考CFF)
    void InitTreeDir();
    //错误消息
    void GetErrorMsg();
    //数据类型
    void GetDataType(int n, char *pszStr);
    //获取数据类型和含义
    void GetDataTypeAndVal(IN const char *szPEHeaderType, IN void *p, IN char *szBuf, OUT char *pszDataType, OUT void* pVal);
    void GetDataTypeAndVal(IN const char *szPEHeaderType, IN void *p, IN char *szBuf, OUT char *pszDataType, OUT DWORD &dwVal);
    //字段含义
    void GetMeaning(WORD n, char *pszMeaning);
    //设置公共标题部分，以及声明公共的一些变量
    void SetCommonHeaderInfo(int n);
    //向表格添加数据
    void AddInfo(int &nOffset, int &nRow, const char *pszName, int n = 4);
    void AddInfoEx(int& nOffset, int& nRow, const char* pszName, PVOID p, const char *pszHeaderType, int n = 4);
    //添加节表数据
    void AddSectionInfo(int nRow);
    //格式化节表返回值
    void FormatSectionRetVal(IN PIMAGE_SECTION_HEADER pSH, IN const char* pszColumn, OUT DWORD &dwVal);

//文件操作
protected:
    //拖动进入事件
    void dragEnterEvent(QDragEnterEvent *event);
    //拖动释放事件
    void dropEvent(QDropEvent *event);
    //打开文件
    int MyOpenFile();

//PE相关
private:
    //获取模块基址
    BOOL GetImageBase();
    //是否是PE文件
    BOOL IsPE();
    //查看Dos Header
    void ShowDosHeaderInfo();
    //查看Nt Headers
    void ShowNtHeadersInfo();
    //查看File Header
    void ShowFileHeaderInfo();
    //查看Optional Header
    void ShowOptionalHeaderInfo();
    //查看Data Directories [x]
    void ShowDataDirectoriesXInfo();
    //查看Section Headers [x]
    void ShowSectionDirectoryXInfo();
    //获取第一个Section节表地址
    void GetFirstSectionHeader();
    //查看VA RVA FOA转换
    void ShowAddressConverterInfo();
    size_t RVAToOffset(size_t stRVA, PVOID lpFileBuf);
    size_t Offset2VA(size_t stOffset, PVOID lpFileBuf);
    //查看导入表
    void ShowImportDirectoryInfo();
    //重定位表
    void ShowRelocationDirectoryInfo();
    //遍历重重定位项
    void QueryRelocationItems();
    //解析导出表
    void ShowExportDirInfo();
    void InitExportDir();
    void AddExportDirMember(PVOID p, DWORD dwFOA);
    void FormatExportDirMember(IN PIMAGE_EXPORT_DIRECTORY pExportDir, IN const char* pszName, OUT DWORD& dwVal, OUT char* pszDataType);
    void AddExportDirFunc(IN PIMAGE_EXPORT_DIRECTORY pExportDir);
    void AddExportFuncItem(PEXPORT_FUNC p, size_t nRow);
    //TLS
    void ShowTlsDirectory();
    void InitTlsDir();
    void FormatTlsDirMember(IN PIMAGE_TLS_DIRECTORY pTlsDir, IN const char* pszName, OUT DWORD& dwVal, OUT char* pszDataType);

private:
    Ui::MainWindow *ui;

//文件相关
private:
    QStandardItemModel *m_StdItemModel;     //左侧树形结构(参考CFF)
    QString m_filePath;                     //文件全路径
    QString m_oldFilePath;                  //之前的文件全路径
    HANDLE m_hFileRead;     //文件读句柄
    HANDLE m_hFileWrite;    //文件写句柄
    HANDLE m_hMapFile;      //文件映射句柄

//PE相关
private:
    HANDLE m_hImageBase;             //模块基址
    PIMAGE_DOS_HEADER m_pDH;         //Dos Header
    PIMAGE_NT_HEADERS m_pNTH;        //Nt Headers
    PIMAGE_FILE_HEADER m_pFH;        //File Header
    PIMAGE_OPTIONAL_HEADER32 m_pOH32; //32位可选头
    PIMAGE_SECTION_HEADER m_pSH;     //节表头 Section Header

    vector<DWORD> m_vctExportOffset; //导出表函数文件偏移（FOA）

signals:
public slots:
    //TreeView 单击事件
    void OnTreeViewClickSlot(const QModelIndex &index);
    //打开文件
    //void OpenFileSlot();
    //添加偏移数据
    void AddSectionOffsetSlot(const QModelIndex);
    void VA2RVA2FASlot();
    void RVA2FA2VASlot();
    void FA2VA2RVASlot();
    //导出表
    void AddExportFuncOffsetSlot(const QModelIndex);
    //导入表 tw_peid_1 clicked事件，显示模块偏移和导入函数
    void ShowImpDirOffsetAndFuncSlot(const QModelIndex);
    //导入表 tw_peid_2 clicked事件，显示导入函数偏移
    void ShowImpDirFuncOffsetSlot(const QModelIndex);
    //添加重定位页偏移
    void AddRelocationOffsetSlot(const QModelIndex);
    //添加重定位项偏移
    void AddRelocItemOffsetSlot(const QModelIndex);
    
};

#endif // MAINWINDOW_H
