#pragma once

#ifndef _WINDOWS_H
#define _WINDOWS_H
#include <Windows.h>
#endif

class MyCmd
{
public:
	/*
	   socket 连接套接字
	   pData  服务端命令参数
	   nlen   命令参数长度
	*/
	MyCmd(SOCKET socket, char* pData, int nlen);
	~MyCmd();

	bool CreateCmd();//只创建一次，加个标记区分
	void SetCmdLine(char* pData, int nlen);

	void RecycleCount()
	{
		m_nCount = 0;
	}

private:
	SOCKET m_socket;
	char* m_pData;
	int m_nlen;

	ULONG m_nCount = 0; //记录数据总的大小
};

