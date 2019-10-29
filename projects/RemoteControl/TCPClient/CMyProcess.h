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
	bool EnumProcess(); //ö�ٽ���
	void UpRole();      //������Ȩ
	void Terminate(const DWORD &pID); //ǿ�ƽ�������

	wstringstream& GetWstream(){ return wstream; }
	wstring& GetWstr() { return wstr; }

private:
	SOCKET m_socket;
	wstringstream wstream;
	wstring wstr;
};

