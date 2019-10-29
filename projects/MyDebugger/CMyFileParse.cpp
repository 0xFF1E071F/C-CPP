#include "CMyFileParse.h"

CMyFileParse::CMyFileParse()
{
	m_hFile = NULL;
	m_hFileMapHandle = NULL;
	m_CPathFileName = NULL;

	m_pFileDosHeader = NULL;
	m_pFileNtHeader = NULL;
	m_pFileSectionHeader = NULL;
}

CMyFileParse::~CMyFileParse()
{
	if (m_CPathFileName)
	{
		delete[] m_CPathFileName;
		m_CPathFileName = NULL;
	}
}

BOOL CMyFileParse::ParseFile(const char* pszFileName)
{
	int nLength = strlen(pszFileName);
	m_CPathFileName = new char[nLength + sizeof(char)];
	strcpy(m_CPathFileName, pszFileName);

	//���ļ�
	m_hFile = CreateFile(pszFileName, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		m_UI.Message("�ļ���ʧ�ܣ� err:%d\r\n", GetLastError());
		return FALSE;
	}

	//ӳ���ļ�
	m_hFileMapHandle = CreateFileMapping(m_hFile, 
		NULL, 
		PAGE_READWRITE, 
		0, 
		0, 
		NULL);
	if (m_hFileMapHandle == NULL)
	{
		m_UI.Message("�����ļ�ӳ��ʧ�ܣ� err:%d\r\n", GetLastError());
		CloseHandle(m_hFile);
		return FALSE;
	}

	//ImageBase
	m_hFileMapImageBase = MapViewOfFile(m_hFileMapHandle, 
		FILE_MAP_ALL_ACCESS, 
		0, 
		0, 
		0); 
	if (m_hFileMapImageBase == NULL)
	{
		m_UI.Message("�ļ�ӳ��ʧ�ܣ� err:%d\r\n", GetLastError());
		CloseHandle(m_hFileMapHandle);
		CloseHandle(m_hFile);
		return FALSE;
	}

	//�ж��ǲ�����Ч��PE�ļ�
	if (!IsPeFile(m_hFileMapImageBase))
	{
		m_UI.Message("������Ч��PE�ļ��� err:%d\r\n", GetLastError());
		return FALSE;
	}

	//ж���ڴ�ӳ��
	if (m_hFileMapImageBase)
	{
		UnmapViewOfFile(m_hFileMapImageBase);
	}
	//�ر��ļ�ӳ����
	if (m_hFileMapHandle)
	{
		CloseHandle(m_hFileMapHandle);
	}

	//�ر��ļ����
	if (m_hFile)
	{
		CloseHandle(m_hFile);
	}

	return TRUE;
}

BOOL CMyFileParse::IsPeFile(LPVOID StartAddress)
{
	//�������������һЩ�򵥵��жϣ����ϸ��
	//IMAGE_DOS_SIGNATURE 0x5a4d = MZ
	//IMAGE_NT_SIGNATURE  0x5045 = PE
	//DOSͷ
	m_pFileDosHeader = (IMAGE_DOS_HEADER*)StartAddress;
	if (m_pFileDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return FALSE;

	//NTͷ
	m_pFileNtHeader = (IMAGE_NT_HEADERS*)((int)StartAddress + m_pFileDosHeader->e_lfanew);
	if (m_pFileNtHeader->Signature != IMAGE_NT_SIGNATURE)
		return FALSE;

	//�ļ�ͷ
	m_pFileHeader = &m_pFileNtHeader->FileHeader;
	//�ڱ�ͷ
	m_pFileSectionHeader = (IMAGE_SECTION_HEADER*)(
		(int)StartAddress + m_pFileDosHeader->e_lfanew
		+ sizeof(DWORD)
		+ m_pFileHeader->SizeOfOptionalHeader
		+ sizeof(IMAGE_FILE_HEADER));
	//��ѡͷ
	m_pOptionalHeader = &m_pFileNtHeader->OptionalHeader;
	//����Ŀ¼
	m_pDataDirectory = m_pOptionalHeader->DataDirectory;

	return TRUE;
}

/*
���ܣ�����������
������
HANDLE hProcess��Ҫ���������Ľ��̾��
char* szDllName��DLL�ļ���
LPVOID StartAddress���ļ����ڴ��е��׵�ַ
*/
void CMyFileParse::ParseExportTable(HANDLE hProcess, char* szDllName, LPVOID StartAddress)
{
	//�����������ڼ䣬���浼������ڴ治�ͷ�
	m_EptInfor.pEptTableFun = new std::list<ExportFunInfor>;
	//ȡ������Ŀ¼�еĵ�����RVA
	DWORD ExportAddress = m_pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	//�ж��Ƿ���ڵ�����
	if (!ExportAddress)
		return;

	//����
	char szDllNameTmp[64] = { 0 };
	DWORD dwOldProto = 0;

	//ֱ��ȡ������Ľṹ�����ص��ڴ��е��������ʵ��ַVA
	int nExportAddr = m_pDataDirectory->VirtualAddress + (DWORD)StartAddress;
	//��ȡ������
	ReadProcessMemory(hProcess, 
		(LPVOID)nExportAddr,
		&m_DllExportTable, 
		sizeof(IMAGE_EXPORT_DIRECTORY), 
		&dwOldProto);

	//����������ַVA
	DWORD dwFunAddrVA = (DWORD)(m_DllExportTable.AddressOfFunctions + (DWORD)StartAddress);
	//��������ű�VA
	DWORD dwFunNameOrdVA = (DWORD)(m_DllExportTable.AddressOfNameOrdinals + (DWORD)StartAddress);
	//��������ַVA
	DWORD dwFunNameVA = (DWORD)(m_DllExportTable.AddressOfNames + (DWORD)StartAddress);
	//DLL�ļ���VA
	DWORD dwDllNameAddrVA = (DWORD)(m_DllExportTable.Name + (DWORD)StartAddress);
	//��ȡDLL�ļ���
	ReadProcessMemory(hProcess, 
		(LPVOID)dwDllNameAddrVA,
		(LPVOID)szDllName, 
		64,
		&dwOldProto);

	size_t k = 0;
	//��¼DLL�ļ���
	while (*(szDllName + k) != '\0')
		k++;
	//��������¼��ص�DLL�ļ�������.dll��׺���������ﱣ��DLL��ʱ������Ҫ.dll��׺
	strncpy(szDllNameTmp, szDllName, k - 4);
	memcpy(m_EptInfor.szDllTableName, szDllNameTmp, k - 4);

	DWORD i, j;
	//���к�����ַƫ������
	DWORD dwFunOffsetRVA = 0;
	WORD nFunNameAddr = 0;
	DWORD dwCurAddress = 0;
	char szFunName[MAX_PATH] = { 0 };

	//��ʼ���ڲ�������ṹ
	ExportFunInfor pEptFunInfor;
	ZeroMemory(&pEptFunInfor, sizeof(ExportFunInfor));

	//����������ѭ����ȡ����
	for (i = 0; i < m_DllExportTable.NumberOfFunctions; i++)
	{
		//��ȡÿ���������RVAƫ��
		ReadProcessMemory(hProcess, 
			(LPCVOID)dwFunAddrVA,
			&dwFunOffsetRVA,
			sizeof(int), 
			&dwOldProto);
		//������Ч��ַ������
		if (0 == dwFunOffsetRVA)
		{
			dwFunAddrVA += sizeof(DWORD);
			continue;
		}

		//�����Ժ������Ƶ����ĵ�ַ����
		for (j = 0; j < m_DllExportTable.NumberOfNames; j++)
		{
			//��������������������������ŵ����ĺ���
			ReadProcessMemory(hProcess, 
				(LPVOID)(dwFunNameOrdVA + j * 2 /*sizeof(WORD)*/),//����Ѱַ��ʽ
				&nFunNameAddr, 
				sizeof(WORD), 
				&dwOldProto);

			//��ű��ֵΪ��ַ���������Ҳ����˵ͨ������ڵ�ַ�����ҵ���
			if (i == nFunNameAddr)
			{
				memset(szFunName, 0, MAX_PATH);
				//������ַRVA
				DWORD dwAddress = j * sizeof(DWORD) + dwFunNameVA;
				ReadProcessMemory(hProcess, 
					(LPVOID)(dwAddress),
					&dwCurAddress, 
					sizeof(DWORD),
					&dwOldProto);

				//������ַVA
				dwCurAddress += (DWORD)StartAddress;
				//ȡ�ú�������
				ReadProcessMemory(hProcess, 
					(LPVOID)(dwCurAddress),
					szFunName, 
					MAX_PATH, 
					&dwOldProto);
				break;
			}
		}

		//���溯������
		if (szFunName != NULL)
		{
			strcpy(pEptFunInfor.szFunName, szFunName);
		}
		//�������
		pEptFunInfor.nExportNumber = i + m_DllExportTable.Base;
		//�����ַ
		pEptFunInfor.dwFunAddress = dwFunOffsetRVA + (int)StartAddress;
		//�����ڲ�ṹ
		m_EptInfor.pEptTableFun->push_back(pEptFunInfor);
		//����������
		dwFunAddrVA += sizeof(DWORD);
	}
	//�������ṹ
	m_ExportTableList.push_back(m_EptInfor);
}

/*
���ܣ����������
������
HANDLE m_hProcess��Ҫ���������Ľ��̾��
LPVOID StartAddress ���ļ����ڴ��е��׵�ַ
*/
void CMyFileParse::ParseImportTable(HANDLE m_hProcess, LPVOID StartAddress)
{
}
