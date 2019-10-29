#pragma once
#include "CMyUI.h"
#include <Windows.h>
#include <list>
using namespace std;
/*
文件解析类，该项目中文件针对PE文件格式
*/

//导入表结构
struct ImportFunInfor
{
	char szFunName[MAX_PATH];          //函数名称
	unsigned int nFunAddress;    //函数地址
};
struct ImportTableInfor
{
	HMODULE hModule;             //模块
	DWORD ModuleAddress;         //模块地址
	char DllModuleName[MAX_PATH];      //DLL文件名
	std::list<ImportFunInfor>* pFunInfoList; //导入表函数链表
};

//导出表结构
struct ExportFunInfor
{
	char  szFunName[MAX_PATH];       //函数名称导出
	int   nExportNumber;       //函数序号导出
	DWORD dwFunAddress;        //函数所在地址
};
struct ExportTableInfor       //导出函数表
{
	char szDllTableName[MAX_PATH];                  //DLL
	std::list<ExportFunInfor>* pEptTableFun;  //数据    
};

class CMyFileParse
{
public:
	CMyFileParse();
	~CMyFileParse();

public:
	//分析文件格式,针对PE文件格式
	BOOL ParseFile(const char* pszFileName);
	//判断是否PE格式
	BOOL IsPeFile(LPVOID StartAddress);
	//解析导出表
	void ParseExportTable(HANDLE hProcess, char* szDllName, LPVOID StartAddress);
	//解析导入表
	void ParseImportTable(HANDLE m_hProcess, LPVOID StartAddress);
public:
	char* m_CPathFileName;
	//打开文件的句柄
	HANDLE m_hFile;
	//文件映射句柄
	HANDLE m_hFileMapHandle;
	//文件映射
	LPVOID m_hFileMapImageBase;

	//Dos头
	IMAGE_DOS_HEADER* m_pFileDosHeader;
	//NT头
	IMAGE_NT_HEADERS* m_pFileNtHeader;
	//File头
	IMAGE_FILE_HEADER* m_pFileHeader;
	//可选头
	IMAGE_OPTIONAL_HEADER32* m_pOptionalHeader;
	//节表位置
	IMAGE_SECTION_HEADER* m_pFileSectionHeader;
	//IMAGE_DATA_DIRECTORY位置
	IMAGE_DATA_DIRECTORY* m_pDataDirectory;
	//导入表头
	IMAGE_IMPORT_DESCRIPTOR m_DllImportTableHeader;
	//导出表
	IMAGE_EXPORT_DIRECTORY m_DllExportTable;

public:
	CMyUI m_UI; //UI封装显示    
	ExportTableInfor m_EptInfor; //导出表内部结构
	std::list<ImportTableInfor> m_ImportTableList; //存储多个导入表内部结构
	std::list<ExportTableInfor> m_ExportTableList; //存储多个导出表内部结构
};

