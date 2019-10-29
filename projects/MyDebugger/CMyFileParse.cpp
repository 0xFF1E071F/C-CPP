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

	//打开文件
	m_hFile = CreateFile(pszFileName, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		m_UI.Message("文件打开失败， err:%d\r\n", GetLastError());
		return FALSE;
	}

	//映射文件
	m_hFileMapHandle = CreateFileMapping(m_hFile, 
		NULL, 
		PAGE_READWRITE, 
		0, 
		0, 
		NULL);
	if (m_hFileMapHandle == NULL)
	{
		m_UI.Message("创建文件映射失败， err:%d\r\n", GetLastError());
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
		m_UI.Message("文件映射失败， err:%d\r\n", GetLastError());
		CloseHandle(m_hFileMapHandle);
		CloseHandle(m_hFile);
		return FALSE;
	}

	//判断是不是有效的PE文件
	if (!IsPeFile(m_hFileMapImageBase))
	{
		m_UI.Message("不是有效的PE文件， err:%d\r\n", GetLastError());
		return FALSE;
	}

	//卸载内存映射
	if (m_hFileMapImageBase)
	{
		UnmapViewOfFile(m_hFileMapImageBase);
	}
	//关闭文件映射句柄
	if (m_hFileMapHandle)
	{
		CloseHandle(m_hFileMapHandle);
	}

	//关闭文件句柄
	if (m_hFile)
	{
		CloseHandle(m_hFile);
	}

	return TRUE;
}

BOOL CMyFileParse::IsPeFile(LPVOID StartAddress)
{
	//这里仅仅进行了一些简单的判断，不深究细节
	//IMAGE_DOS_SIGNATURE 0x5a4d = MZ
	//IMAGE_NT_SIGNATURE  0x5045 = PE
	//DOS头
	m_pFileDosHeader = (IMAGE_DOS_HEADER*)StartAddress;
	if (m_pFileDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return FALSE;

	//NT头
	m_pFileNtHeader = (IMAGE_NT_HEADERS*)((int)StartAddress + m_pFileDosHeader->e_lfanew);
	if (m_pFileNtHeader->Signature != IMAGE_NT_SIGNATURE)
		return FALSE;

	//文件头
	m_pFileHeader = &m_pFileNtHeader->FileHeader;
	//节表头
	m_pFileSectionHeader = (IMAGE_SECTION_HEADER*)(
		(int)StartAddress + m_pFileDosHeader->e_lfanew
		+ sizeof(DWORD)
		+ m_pFileHeader->SizeOfOptionalHeader
		+ sizeof(IMAGE_FILE_HEADER));
	//可选头
	m_pOptionalHeader = &m_pFileNtHeader->OptionalHeader;
	//数据目录
	m_pDataDirectory = m_pOptionalHeader->DataDirectory;

	return TRUE;
}

/*
功能：解析导出表
参数：
HANDLE hProcess：要解析导入表的进程句柄
char* szDllName：DLL文件名
LPVOID StartAddress：文件在内存中的首地址
*/
void CMyFileParse::ParseExportTable(HANDLE hProcess, char* szDllName, LPVOID StartAddress)
{
	//调试器运行期间，保存导出表的内存不释放
	m_EptInfor.pEptTableFun = new std::list<ExportFunInfor>;
	//取得数据目录中的导出表RVA
	DWORD ExportAddress = m_pDataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	//判断是否存在导出表
	if (!ExportAddress)
		return;

	//属性
	char szDllNameTmp[64] = { 0 };
	DWORD dwOldProto = 0;

	//直接取导出表的结构，加载到内存中导出表的真实地址VA
	int nExportAddr = m_pDataDirectory->VirtualAddress + (DWORD)StartAddress;
	//读取导出表
	ReadProcessMemory(hProcess, 
		(LPVOID)nExportAddr,
		&m_DllExportTable, 
		sizeof(IMAGE_EXPORT_DIRECTORY), 
		&dwOldProto);

	//导出函数地址VA
	DWORD dwFunAddrVA = (DWORD)(m_DllExportTable.AddressOfFunctions + (DWORD)StartAddress);
	//函数名序号表VA
	DWORD dwFunNameOrdVA = (DWORD)(m_DllExportTable.AddressOfNameOrdinals + (DWORD)StartAddress);
	//函数名地址VA
	DWORD dwFunNameVA = (DWORD)(m_DllExportTable.AddressOfNames + (DWORD)StartAddress);
	//DLL文件名VA
	DWORD dwDllNameAddrVA = (DWORD)(m_DllExportTable.Name + (DWORD)StartAddress);
	//获取DLL文件名
	ReadProcessMemory(hProcess, 
		(LPVOID)dwDllNameAddrVA,
		(LPVOID)szDllName, 
		64,
		&dwOldProto);

	size_t k = 0;
	//记录DLL文件名
	while (*(szDllName + k) != '\0')
		k++;
	//正常情况下加载的DLL文件名都以.dll后缀结束，这里保存DLL名时，不需要.dll后缀
	strncpy(szDllNameTmp, szDllName, k - 4);
	memcpy(m_EptInfor.szDllTableName, szDllNameTmp, k - 4);

	DWORD i, j;
	//所有函数地址偏移链表
	DWORD dwFunOffsetRVA = 0;
	WORD nFunNameAddr = 0;
	DWORD dwCurAddress = 0;
	char szFunName[MAX_PATH] = { 0 };

	//初始化内部导出表结构
	ExportFunInfor pEptFunInfor;
	ZeroMemory(&pEptFunInfor, sizeof(ExportFunInfor));

	//遍历导出表，循环读取链表
	for (i = 0; i < m_DllExportTable.NumberOfFunctions; i++)
	{
		//获取每个导出项的RVA偏移
		ReadProcessMemory(hProcess, 
			(LPCVOID)dwFunAddrVA,
			&dwFunOffsetRVA,
			sizeof(int), 
			&dwOldProto);
		//忽略无效地址导出项
		if (0 == dwFunOffsetRVA)
		{
			dwFunAddrVA += sizeof(DWORD);
			continue;
		}

		//遍历以函数名称导出的地址数组
		for (j = 0; j < m_DllExportTable.NumberOfNames; j++)
		{
			//根据名称所在索引，查找以序号导出的函数
			ReadProcessMemory(hProcess, 
				(LPVOID)(dwFunNameOrdVA + j * 2 /*sizeof(WORD)*/),//数组寻址公式
				&nFunNameAddr, 
				sizeof(WORD), 
				&dwOldProto);

			//序号表的值为地址表的索引，也就是说通过序号在地址表中找到了
			if (i == nFunNameAddr)
			{
				memset(szFunName, 0, MAX_PATH);
				//函数地址RVA
				DWORD dwAddress = j * sizeof(DWORD) + dwFunNameVA;
				ReadProcessMemory(hProcess, 
					(LPVOID)(dwAddress),
					&dwCurAddress, 
					sizeof(DWORD),
					&dwOldProto);

				//函数地址VA
				dwCurAddress += (DWORD)StartAddress;
				//取得函数名称
				ReadProcessMemory(hProcess, 
					(LPVOID)(dwCurAddress),
					szFunName, 
					MAX_PATH, 
					&dwOldProto);
				break;
			}
		}

		//保存函数名称
		if (szFunName != NULL)
		{
			strcpy(pEptFunInfor.szFunName, szFunName);
		}
		//保存序号
		pEptFunInfor.nExportNumber = i + m_DllExportTable.Base;
		//保存地址
		pEptFunInfor.dwFunAddress = dwFunOffsetRVA + (int)StartAddress;
		//保存内层结构
		m_EptInfor.pEptTableFun->push_back(pEptFunInfor);
		//向后继续遍历
		dwFunAddrVA += sizeof(DWORD);
	}
	//保存外层结构
	m_ExportTableList.push_back(m_EptInfor);
}

/*
功能：解析导入表
参数：
HANDLE m_hProcess：要解析导入表的进程句柄
LPVOID StartAddress ：文件在内存中的首地址
*/
void CMyFileParse::ParseImportTable(HANDLE m_hProcess, LPVOID StartAddress)
{
}
