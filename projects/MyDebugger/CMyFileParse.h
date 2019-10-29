#pragma once
#include "CMyUI.h"
#include <Windows.h>
#include <list>
using namespace std;
/*
�ļ������࣬����Ŀ���ļ����PE�ļ���ʽ
*/

//�����ṹ
struct ImportFunInfor
{
	char szFunName[MAX_PATH];          //��������
	unsigned int nFunAddress;    //������ַ
};
struct ImportTableInfor
{
	HMODULE hModule;             //ģ��
	DWORD ModuleAddress;         //ģ���ַ
	char DllModuleName[MAX_PATH];      //DLL�ļ���
	std::list<ImportFunInfor>* pFunInfoList; //�����������
};

//������ṹ
struct ExportFunInfor
{
	char  szFunName[MAX_PATH];       //�������Ƶ���
	int   nExportNumber;       //������ŵ���
	DWORD dwFunAddress;        //�������ڵ�ַ
};
struct ExportTableInfor       //����������
{
	char szDllTableName[MAX_PATH];                  //DLL
	std::list<ExportFunInfor>* pEptTableFun;  //����    
};

class CMyFileParse
{
public:
	CMyFileParse();
	~CMyFileParse();

public:
	//�����ļ���ʽ,���PE�ļ���ʽ
	BOOL ParseFile(const char* pszFileName);
	//�ж��Ƿ�PE��ʽ
	BOOL IsPeFile(LPVOID StartAddress);
	//����������
	void ParseExportTable(HANDLE hProcess, char* szDllName, LPVOID StartAddress);
	//���������
	void ParseImportTable(HANDLE m_hProcess, LPVOID StartAddress);
public:
	char* m_CPathFileName;
	//���ļ��ľ��
	HANDLE m_hFile;
	//�ļ�ӳ����
	HANDLE m_hFileMapHandle;
	//�ļ�ӳ��
	LPVOID m_hFileMapImageBase;

	//Dosͷ
	IMAGE_DOS_HEADER* m_pFileDosHeader;
	//NTͷ
	IMAGE_NT_HEADERS* m_pFileNtHeader;
	//Fileͷ
	IMAGE_FILE_HEADER* m_pFileHeader;
	//��ѡͷ
	IMAGE_OPTIONAL_HEADER32* m_pOptionalHeader;
	//�ڱ�λ��
	IMAGE_SECTION_HEADER* m_pFileSectionHeader;
	//IMAGE_DATA_DIRECTORYλ��
	IMAGE_DATA_DIRECTORY* m_pDataDirectory;
	//�����ͷ
	IMAGE_IMPORT_DESCRIPTOR m_DllImportTableHeader;
	//������
	IMAGE_EXPORT_DIRECTORY m_DllExportTable;

public:
	CMyUI m_UI; //UI��װ��ʾ    
	ExportTableInfor m_EptInfor; //�������ڲ��ṹ
	std::list<ImportTableInfor> m_ImportTableList; //�洢���������ڲ��ṹ
	std::list<ExportTableInfor> m_ExportTableList; //�洢����������ڲ��ṹ
};

