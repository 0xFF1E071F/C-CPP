#pragma once

#ifndef _WINDOWS_H
#define _WINDOWS_H
#include <Windows.h>
#endif

class MyCmd
{
public:
	/*
	   socket �����׽���
	   pData  ������������
	   nlen   �����������
	*/
	MyCmd(SOCKET socket, char* pData, int nlen);
	~MyCmd();

	bool CreateCmd();//ֻ����һ�Σ��Ӹ��������
	void SetCmdLine(char* pData, int nlen);

	void RecycleCount()
	{
		m_nCount = 0;
	}

private:
	SOCKET m_socket;
	char* m_pData;
	int m_nlen;

	ULONG m_nCount = 0; //��¼�����ܵĴ�С
};

