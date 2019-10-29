#include "CAsio.h"

void CAsio::Accept() {
	std::shared_ptr<CSession> handler = CreateHandler();
	if (handler == nullptr)
	{
		return;
	}
	m_acceptor.async_accept(handler->GetSocket(), [this, handler](const boost::system::error_code& error)
	{
		if (error)
		{
			HandleAcpError(handler, error);
		}

		m_handlers.insert(std::make_pair(handler->GetConnId(), handler));

		emit SendConnSig();

		handler->HandleRead();
		Accept();
	});
}

//主动向客户端发送消息 //            向哪个客户端发      发什么类型消息
void CAsio::SendData(boost::unordered_map<int, std::shared_ptr<CSession>>::iterator iter,
	int msgType)
{
	iter->second->Write(msgType);//向客户端发送数据
}

std::shared_ptr<CSession> CAsio::CreateHandler()
{
	std::lock_guard<std::mutex> guard(GetMutex());
	int connId = m_connIdPool.front();
	m_connIdPool.pop_front();

	std::shared_ptr<CSession> handler = std::make_shared<CSession>(m_ios);

	handler->SetConnId(connId);

	handler->SetCallBackError([this](int connId){ RecycleConnid(connId); });

	return handler;
}

void CAsio::RecycleConnid(int connId)
{
	std::lock_guard<std::mutex> guard(GetMutex());
	if (m_handlers.empty())
	{
		return;
	}
	auto it = m_handlers.find(connId);
	if (it != m_handlers.end())
	{
		m_handlers.erase(it);
	}
	
	m_connIdPool.push_back(connId);
}

void CAsio::HandleAcpError(std::shared_ptr<CSession> eventHanlder, const boost::system::error_code & error)
{
	if (error)
	{
		//关闭socket，移除读事件处理器
		eventHanlder->CloseSocket();
		StopAccept();
	}
}

void CAsio::StopAccept()
{
	boost::system::error_code ec;
	m_acceptor.cancel(ec);
	m_acceptor.close(ec);
	m_ios.stop();
}

