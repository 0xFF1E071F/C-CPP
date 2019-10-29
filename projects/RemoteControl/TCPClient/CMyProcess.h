#pragma once

#ifndef _WINDOWS_H
#define _WINDOWS_H
#include <Windows.h>
#endif

#include <sstream>

using namespace std;

class CMyProcess
{
public:
	CMyProcess(SOCKET sock);
	~CMyProcess();

	void SendProcess();
	bool EnumProcess(); //枚举进程
	void UpRole();      //进程提权
	void Terminate(const DWORD &pID); //强制结束进程

	wstringstream& GetWstream(){ return wstream; }
	wstring& GetWstr() { return wstr; }

private:
	SOCKET m_socket;
	wstringstream wstream;
	wstring wstr;
};

