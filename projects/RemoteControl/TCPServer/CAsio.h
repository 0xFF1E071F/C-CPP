#ifndef CASIO_H
#define CASIO_H

#include <QObject>
#include <list>
#include <memory>
#include <utility>
#include "CSession.h"
#include <boost/asio/io_context.hpp>
#include <boost/unordered_map.hpp>
#include <mutex>

using boost::asio::ip::tcp;

const int MaxConnNum = 65536; //客户端最大连接数

class CAsio : public QObject
{
    Q_OBJECT
public:
	CAsio(boost::asio::io_service& ios, short port) : 
		m_ios(ios), m_acceptor(ios, tcp::endpoint(tcp::v4(), port)), m_connIdPool(MaxConnNum)
	{
		int current = 0;
		std::generate_n(m_connIdPool.begin(), MaxConnNum, [&current] {
			return ++current;
		});
	}
	
	// 监听连接
    void Accept();

	//获取客户端列表集合
	boost::unordered_map<int, std::shared_ptr<CSession>> GetHandlers()
	{
		return m_handlers;
	}

	void SendConn() { emit SendConnSig(); }

	//服务端向客户端主动发消息
	/*
	 socket 哪个客户端
	 msgType 消息类型
	*/
	void SendData(
		boost::unordered_map<int, std::shared_ptr<CSession>>::iterator iter,
		int msgType);

	//屏幕
	void ShowScreen(const char* ptr, int nlen);

	std::mutex& GetMutex() { return m_mutex; }

private:
	std::shared_ptr<CSession> CreateHandler(); //添加连接id到连接池
	void RecycleConnid(int connId);
	void HandleAcpError(std::shared_ptr <CSession> eventHanlder, const boost::system::error_code& error);
	void StopAccept();

private:
	boost::asio::io_service& m_ios;
	tcp::acceptor m_acceptor;
	boost::unordered_map<int, std::shared_ptr<CSession>> m_handlers; // 客户端集合
	std::list<int> m_connIdPool; // 管理客户端连接数
	std::mutex m_mutex;
	
signals:
    void SendConnSig(); // 更新客户端连接列表
	//void SendData();
public slots:
	
};

#endif // CASIO_H