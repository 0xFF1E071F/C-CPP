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

//�ͻ���session������
class CSession
	: public QObject , public std::enable_shared_from_this<CSession>
{
	Q_OBJECT
public:
	CSession(boost::asio::io_context& ioc)
		: m_socket(ioc)
	{
		// ��Ļ
		QObject::connect(this, SIGNAL(SendScreenSig(const char*, int)), pMain, SLOT(ShowScreenSlot(const char*, int)));
		// cmd
		QObject::connect(this, SIGNAL(SendCmdSig(QByteArray)), pMain, SLOT(ShowCmdSlot(QByteArray)));
		//����
		QObject::connect(this, SIGNAL(SendProcessSig(QByteArray)), pMain, SLOT(ShowProcessSlot(QByteArray)));
	}

	~CSession();

	void Start() { HandleRead(); }// ��ʼ����
	tcp::socket& GetSocket() { return m_socket; }// �����׽���
	void SetConnId(int connId) { m_connId = connId; }// ��������id
	int GetConnId() { return m_connId; }// ��������id
	void SetHbCount() { ++m_nHbCount; };
	void SetHbCount(int count) { m_nHbCount = count; }
	size_t GetHbCount() { return m_nHbCount; }//��ȡ��Ϣ����
	void FreeSocket() { HandleError(); }; //�ͻ����ѶϿ����׽����ͷ�
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

	//��������»᷵�أ�1.����������2.transfer_at_leastΪ��(�յ��ض������ֽڼ�����)��3.�д�����
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
					//��ʾ���������ˣ���ʼ����

					int nLength = pHead->length + PKTSIZE;
					char* pData = new char[nLength];
					myBuf.Read((PBYTE)pData, nLength);

					//������Э�飬���ֲ�ͬ����Ϣ����
					tagPacket* tagPkt = (tagPacket*)pData;
					switch (tagPkt->code)
					{
						//��Ļ
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

						//�ļ�
						case MSG_FILE:
						{

						}
						break;

						//cmd
						case MSG_CMD:
						{
							//qDebug() << nLength - PKTSIZE << '\n';
							//�Ȱ�����ͳһ����һ����Ļ��������ȿͻ��˷�����ϣ������
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
							//�ܴ�С
							m_pData = new char[nLength - PKTSIZE];
							memcpy(m_pData, pData + PKTSIZE, nLength - PKTSIZE);
							int totalSize = 0;
							totalSize = atoi((const char*)m_pData);

							if (totalSize == 0)
							{
								continue;
							}

							//��cmd�ܵĻ�������
							int nlen = baBuf.length();
							

							if (totalSize == nlen)
							{
								//��ʾ
								//char* cBuf = baBuf.data();
								emit SendCmdSig(baBuf);

								//��ս��ջ�����
								baBuf.clear();

								//��Ӧ�ͻ��ˣ��ѽ������
								tagPacket pkt;
								pkt.code = MSG_CMD_YES;
								pkt.length = 0;
								Send((char*)&pkt, sizeof(tagPacket));
							}
						}
						break;
						//����
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

						//����
						case MSG_HEARTBEAT:
						{
							SetHbCount(0);
						}
						break;
						}

						//�ͷ���Դ
						if (pData != nullptr) {
							delete pData;
							pData = nullptr;
						}
				}
				else {
					break;
				}

			}
			//Ͷ�ݽ���
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

			//��mySendBuf��ȥ��ͷ��dwIOBytes
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
		char *str = ba.data(); //ֻ������
		int nlen = strlen(str) + 1;//ֻ�����ݵĳ��� '\0'

		//����ռ�
		char* pBuf = new char[PKTSIZE + nlen];//ͷ��������
		memset(pBuf, 0, PKTSIZE + nlen);
		if (pBuf == nullptr){ return; }

		switch (msgType)
		{
			case MSG_CMD:
			{
				//���
				tagPacket* pkt = (tagPacket*)pBuf;
				pkt->code = MSG_CMD;
				pkt->length = nlen;//���ݳ���
				memcpy(pBuf + PKTSIZE, str, nlen-1);

				Send((char*)pBuf, pkt->length + PKTSIZE);	
			}
			break;

			case MSG_PROCESS_TERMINATE:
			{
				//���
				tagPacket* pkt = (tagPacket*)pBuf;
				pkt->code = MSG_PROCESS_TERMINATE;
				pkt->length = nlen;//���ݳ���
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
				//��Ļ���ڴ���
				bSend = true;

				//����鿴��Ļ
				pkt.code = MSG_SCREEN;
				pkt.length = 0;
				Send((char*)&pkt, sizeof(tagPacket));
			}
			break;
			case MSG_SCREEN_CLOSE:
			{
				bSend = false;
				//�������ͽ�������
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
				//��cmd����
				//cmdWndOpen = true;

				//�״λ�Ӧ�����ݽ������
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
	    pBuf ͷ��������
		nLength ���ݵĳ���
	*/
	void Send(char* pBuf, int nLength) {
		mySendBuf.Write((PBYTE)pBuf, nLength);
		//��֤���Ͷ���ֻ��һ���߳��ڷ�������
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

	tcp::socket m_socket;        //�׽���
	int m_connId;               //����id
	enum { max_length = 1024 }; //ÿ�ν�������ֽ���
	char data_[max_length];     //������

	CBuffer myBuf;              //���ջ�����
	CBuffer mySendBuf;          //���ͻ�����
	CBuffer myCmdBuf;           //CMD���ջ�����
	int m_nlen;                   //CMD��ʼ�������������ݳ���
	QByteArray baBuf;
	//std::string m_cmdBuf;
	//std::stringstream ssBuf;
	bool cmdWndOpen = false;    //���ڴ�״̬��ǣ�falseδ�򿪣�true��
	bool m_sendout = false;     //�ͻ��������˷������ݣ�false ��û�з�����ϣ�true�ѷ������

	size_t m_nHbCount = 0;      //�������������ӵĴ�����0��������(Ĭ��)����3��δ��������ر�����

	//int m_msgType; // Ҫ���͵���Ϣ����

	std::function<void(int)> m_callbackError;

	void* m_pData;

	bool bSend = false; //������false,true����

signals:
	//void SendScreenSig(const char*, int); // ��Ļ
	void SendScreenSig(const char*, int nlen); // ��Ļ
	void SendCmdSig(QByteArray); // CMD
	void SendProcessSig(QByteArray);//����
};