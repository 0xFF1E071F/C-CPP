#pragma once

#include <ws2tcpip.h>
#include <WinSock2.h>

#ifndef _WINDOWS_H
#define _WINDOWS_H
#include <Windows.h>
#endif // !_WINDOWS_H

class MyCmd;
class CMyProcess;
class DskCapture;

class CTcpSocket
{
public:
    CTcpSocket();
    ~CTcpSocket();

    bool StartUp();
    bool CreateAndConnect(int nPort);
	bool RecvThread();
    int Send(SOCKET s, char* pData, int nLength);
    int Recv(SOCKET s, char* pData, int nLength);
    bool Close(SOCKET s);
	bool RecvData(SOCKET s, char* pBuf, int nData);
	bool SendBitmap(char *pBuf, int nSize);

public:
	MyCmd* m_pCmd;
	CMyProcess* m_pProcess;
	DskCapture* m_pDskCapture;

	SOCKET m_ConnectSocket;
	bool bSend = false;
};
