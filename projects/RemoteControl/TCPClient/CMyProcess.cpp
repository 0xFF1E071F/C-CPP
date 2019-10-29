#include "CMyProcess.h"
#include <Tlhelp32.h>
#include <tchar.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;

CMyProcess::CMyProcess(SOCKET sock)
	:m_socket(sock)
{
}

CMyProcess::~CMyProcess()
{
}

std::string wstringToString(const std::wstring& wstr)
{
	LPCWSTR pwszSrc = wstr.c_str();
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
		return std::string("");

	char* pszDst = new char[nLen];
	if (!pszDst)
		return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	std::string str(pszDst);
	delete[] pszDst;
	pszDst = NULL;

	return str;
}

void CMyProcess::SendProcess()
{
	bool bRet = EnumProcess();
	if (!bRet)
	{
		return;
	}

	//发送进程数据到服务端
	std::string str = wstringToString(wstr);
	const char* pData = str.c_str();
	int nlen = str.length();
	
	//SendData(m_socket, MSG_PROCESS, pData, nlen);
}

bool CMyProcess::EnumProcess()
{
	UpRole();

	wptree pt, items;

	HANDLE hProcessSnap;
	BOOL bRet = FALSE;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return bRet;
	}

	PROCESSENTRY32  pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);

	int i = 1;
	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			//这样写，boost ptree 直接根据 . 把json分层了
			/*std::wstring pName = pe32.szExeFile;
			std::wstring pId = to_wstring(pe32.th32ProcessID);
			try
			{
				aryProcess.put(pName, pId);
			}
			catch (ptree_error pterr)
			{
				pterr.what();
				return false;
			}	*/


			std::wstring pName = pe32.szExeFile;
			std::wstring pId = to_wstring(pe32.th32ProcessID);

			wptree item;
			item.put(L"Name", pName);
			item.put(L"ID", pId);
			items.push_back(std::make_pair(to_wstring(i), item));

			i++;
		} while (Process32Next(hProcessSnap, &pe32));
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	pt.add_child(L"aryProcess", items);

	write_json(wstream, pt, false);
	wstr = wstream.str();

	CloseHandle(hProcessSnap);

	return (bRet);
}

void CMyProcess::UpRole()
{
	HANDLE pHandle = GetCurrentProcess();
	HANDLE tHandle;
	// 打开当前进程的访问令牌
	BOOL ret = OpenProcessToken(pHandle, TOKEN_ALL_ACCESS, &tHandle);
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	// 获取描述权限的LUID，SE_DEBUG_NAME调试权限名字
	ret = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// 调整访问令牌的权限
	ret = AdjustTokenPrivileges(tHandle, FALSE, &tp, sizeof(tp), NULL, NULL);
	CloseHandle(tHandle);
}

void AdjustPrivilege()
{
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
		{
			AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		}
		CloseHandle(hToken);
	}
}

void CMyProcess::Terminate(const DWORD & pID)
{
	AdjustPrivilege();
	//== TerminateProcess ==//根据进程ID强杀
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
	if (hProcess)
	{
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
	}
}
