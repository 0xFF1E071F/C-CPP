#pragma once

#include <boost/asio.hpp>
#include <boost/unordered_map.hpp>
#include <qobject.h>
#include "CBuffer.h"
#include "CRtlCompress.h"
#include "../common/common.h"
#include <fstream>
#include <qbytearray.h>

using boost::asio::ip::tcp;

#define PKTSIZE sizeof(tagPacket)

extern QObject* pMain;

//客户端session集合类
class CSession
	: public QObject , public std::enable_shared_from_this<CSession>
{
	Q_OBJECT
public:
	CSession(boost::asio::io_context& ioc)
		: m_socket(ioc)
	{
		// 屏幕
		QObject::connect(this, SIGNAL(SendScreenSig(const char*, int)), pMain, SLOT(ShowScreenSlot(const char*, int)));
		// cmd
		QObject::connect(this, SIGNAL(SendCmdSig(QByteArray)), pMain, SLOT(ShowCmdSlot(QByteArray)));
		//进程
		QObject::connect(this, SIGNAL(SendProcessSig(QByteArray)), pMain, SLOT(ShowProcessSlot(QByteArray)));
	}

	~CSession();

	void Start() { HandleRead(); }// 开始接收
	tcp::socket& GetSocket() { return m_socket; }// 返回套接字
	void SetConnId(int connId) { m_connId = connId; }// 设置连接id
	int GetConnId() { return m_connId; }// 返回连接id
	void SetHbCount() { ++m_nHbCount; };
	void SetHbCount(int count) { m_nHbCount = count; }
	size_t GetHbCount() { return m_nHbCount; }//获取消息类型
	void FreeSocket() { HandleError(); }; //客户端已断开，套接字释放
	bool GetbSend() { return bSend; }

	void CloseSocket()
	{
		boost::system::error_code ec;
		m_socket.shutdown(tcp::socket::shutdown_send, ec);
		m_socket.close(ec);
	}

	template<typename F>
	void SetCallBackError(F f)
	{
		m_callbackError = f;
	}

	//三种情况下会返回：1.缓冲区满；2.transfer_at_least为真(收到特定数量字节即返回)；3.有错误发生
	void HandleRead()
	{
		auto self(shared_from_this());
		m_socket.async_read_some(boost::asio::buffer(data_, max_length),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				HandleError(ec);
			}
			
			myBuf.Write((PBYTE)data_, length);

			while (myBuf.GetBufferLen() >= PKTSIZE) {
				tagPacket* pHead = (tagPacket*)myBuf.GetBuffer(0);

				if (pHead->length + PKTSIZE <= myBuf.GetBufferLen()) {
					//表示包收完整了，开始处理

					int nLength = pHead->length + PKTSIZE;
					char* pData = new char[nLength];
					myBuf.Read((PBYTE)pData, nLength);

					//解析包协议，区分不同的消息类型
					tagPacket* tagPkt = (tagPacket*)pData;
					switch (tagPkt->code)
					{
						//屏幕
						case MSG_SCREEN:
						{
							//m_pData = new char[nLength - PKTSIZE];
							//memcpy(m_pData, pData + PKTSIZE, nLength - PKTSIZE);	
							//emit SendScreenSig((const char*)m_pData, nLength - PKTSIZE);
							BYTE *pUncompressData = NULL;
							DWORD dwUncompressDataLength = 0;

							CRtlCompress RtlCompress;
							RtlCompress.UncompressData((BYTE*)pData + PKTSIZE, nLength - PKTSIZE, &pUncompressData, &dwUncompressDataLength);

							m_pData = new char[dwUncompressDataLength];
							memcpy(m_pData, pUncompressData, dwUncompressDataLength);
							emit SendScreenSig((const char*)m_pData, dwUncompressDataLength);

							if (pUncompressData != nullptr)
							{
								delete pUncompressData;
								pUncompressData = nullptr;
							}

							if (GetbSend())
							{
								tagPacket pkt;
								pkt.code = MSG_SCREEN;
								pkt.length = 0;
								Send((char*)&pkt, sizeof(tagPacket));
							}
							
						}
						break;

						//文件
						case MSG_FILE:
						{

						}
						break;

						//cmd
						case MSG_CMD:
						{
							//qDebug() << nLength - PKTSIZE << '\n';
							//先把数据统一放入一个大的缓冲区，等客户端发送完毕，再输出
							//myCmdBuf.Write((PBYTE)pData + PKTSIZE, nLength - PKTSIZE);
							//std::string stmp = std::string(pData + PKTSIZE, nLength - PKTSIZE);
							//OutputDebugStringA(stmp.c_str());
							//m_cmdBuf += stmp;
							//ssBuf << stmp;
							//OutputDebugStringA(ssBuf.str().c_str());
							QByteArray baTmp = QByteArray(pData + PKTSIZE, nLength - PKTSIZE);
							baBuf += baTmp;
							//char* cBuf = baBuf.data();
						}
						break;

						case MSG_CMD_STOP:
						{
							//总大小
							m_pData = new char[nLength - PKTSIZE];
							memcpy(m_pData, pData + PKTSIZE, nLength - PKTSIZE);
							int totalSize = 0;
							totalSize = atoi((const char*)m_pData);

							if (totalSize == 0)
							{
								continue;
							}

							//读cmd总的缓冲数据
							int nlen = baBuf.length();
							

							if (totalSize == nlen)
							{
								//显示
								//char* cBuf = baBuf.data();
								emit SendCmdSig(baBuf);

								//清空接收缓冲区
								baBuf.clear();

								//回应客户端，已接收完毕
								tagPacket pkt;
								pkt.code = MSG_CMD_YES;
								pkt.length = 0;
								Send((char*)&pkt, sizeof(tagPacket));
							}
						}
						break;
						//进程
						case MSG_PROCESS:
						{
							std::ofstream ofile("TestProcess.txt");
							if (!ofile)
							{
								return;
							}
							ofile.write(pData + PKTSIZE, nLength - PKTSIZE);
							ofile.close();

							QByteArray baBuf = QByteArray(pData + PKTSIZE, nLength - PKTSIZE);
							emit SendProcessSig(baBuf);
						}

						//心跳
						case MSG_HEARTBEAT:
						{
							SetHbCount(0);
						}
						break;
						}

						//释放资源
						if (pData != nullptr) {
							delete pData;
							pData = nullptr;
						}
				}
				else {
					break;
				}

			}
			//投递接收
			HandleRead();
			
		});
	}

	void HandleWrite()
	{
		auto self(shared_from_this());
		boost::asio::async_write(m_socket, boost::asio::buffer(mySendBuf.GetBuffer(), mySendBuf.GetBufferLen()),
			[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				HandleError(ec);
			}

			//将mySendBuf中去掉头部dwIOBytes
			mySendBuf.Delete(length);

			if (mySendBuf.GetBufferLen() > 0) {
				HandleWrite();
			}
		});
	}

	void Write(int msgType, QString sline)
	{
		if (sline.isEmpty()){ return; }

		//QString -> char*
		QByteArray ba = sline.toLocal8Bit();
		char *str = ba.data(); //只是数据
		int nlen = strlen(str) + 1;//只是数据的长度 '\0'

		//分配空间
		char* pBuf = new char[PKTSIZE + nlen];//头部和数据
		memset(pBuf, 0, PKTSIZE + nlen);
		if (pBuf == nullptr){ return; }

		switch (msgType)
		{
			case MSG_CMD:
			{
				//组包
				tagPacket* pkt = (tagPacket*)pBuf;
				pkt->code = MSG_CMD;
				pkt->length = nlen;//数据长度
				memcpy(pBuf + PKTSIZE, str, nlen-1);

				Send((char*)pBuf, pkt->length + PKTSIZE);	
			}
			break;

			case MSG_PROCESS_TERMINATE:
			{
				//组包
				tagPacket* pkt = (tagPacket*)pBuf;
				pkt->code = MSG_PROCESS_TERMINATE;
				pkt->length = nlen;//数据长度
				memcpy(pBuf + PKTSIZE, str, nlen - 1);

				Send((char*)pBuf, pkt->length + PKTSIZE);
			}
		}

		if (pBuf != nullptr)
		{
			delete[] pBuf;
			pBuf = nullptr;
		}
	}

	void Write(int msgType)
	{
		tagPacket pkt;
		switch (msgType)
		{
			case MSG_SCREEN:
			{
				//屏幕窗口打开了
				bSend = true;

				//请求查看屏幕
				pkt.code = MSG_SCREEN;
				pkt.length = 0;
				Send((char*)&pkt, sizeof(tagPacket));
			}
			break;
			case MSG_SCREEN_CLOSE:
			{
				bSend = false;
				//结束发送截屏数据
				pkt.code = MSG_SCREEN_CLOSE;
				pkt.length = 0;
				Send((char*)&pkt, sizeof(tagPacket));
			}
			break;
			case MSG_FILE:
			{

			}
			break;
			case MSG_CMD:
			{
				//pkt.code = MSG_CMD;
				//pkt.length = 0;
				//strcpy(pkt.szData, "\n");

				//Send((char*)&pkt, sizeof(tagPacket));
				//HandleRead();
			}
			break;
			case MSG_CMD_YES:
			{
				//打开cmd窗口
				//cmdWndOpen = true;

				//首次回应，数据接收完毕
				//tagPacket pkt;
				//pkt.code = MSG_CMD_YES;
				//pkt.length = 0;
				//Send((char*)&pkt, sizeof(tagPacket));
			}
			break;
			case MSG_PROCESS:
			{
				//bSend = false;
				pkt.code = MSG_PROCESS;
				pkt.length = 0;
				Send((char*)&pkt, sizeof(tagPacket));
			}
			break;
		}
	}



private:
	/*
	    pBuf 头部和数据
		nLength 数据的长度
	*/
	void Send(char* pBuf, int nLength) {
		mySendBuf.Write((PBYTE)pBuf, nLength);
		//保证发送队列只有一个线程在发送数据
		if (mySendBuf.GetBufferLen() == nLength) {
			HandleWrite();
		}
	}

	void HandleError()
	{
		CloseSocket();
		if (m_callbackError)
		{
			m_callbackError(m_connId);
		}
	}

	void HandleError(const boost::system::error_code& ec)
	{
		if (ec)
		{
		}
		CloseSocket();
		if (m_callbackError)
		{
			m_callbackError(m_connId);
		}
	}

	tcp::socket m_socket;        //套接字
	int m_connId;               //连接id
	enum { max_length = 1024 }; //每次接收最大字节数
	char data_[max_length];     //缓冲区

	CBuffer myBuf;              //接收缓冲区
	CBuffer mySendBuf;          //发送缓冲区
	CBuffer myCmdBuf;           //CMD接收缓冲区
	int m_nlen;                   //CMD初始化发送来的数据长度
	QByteArray baBuf;
	//std::string m_cmdBuf;
	//std::stringstream ssBuf;
	bool cmdWndOpen = false;    //窗口打开状态标记，false未打开，true打开
	bool m_sendout = false;     //客户端向服务端发送数据，false 还没有发送完毕，true已发送完毕

	size_t m_nHbCount = 0;      //心跳包尝试连接的次数，0正常连接(默认)，超3次未连接上则关闭连接

	//int m_msgType; // 要发送的消息类型

	std::function<void(int)> m_callbackError;

	void* m_pData;

	bool bSend = false; //不发送false,true发送

signals:
	//void SendScreenSig(const char*, int); // 屏幕
	void SendScreenSig(const char*, int nlen); // 屏幕
	void SendCmdSig(QByteArray); // CMD
	void SendProcessSig(QByteArray);//进程
};