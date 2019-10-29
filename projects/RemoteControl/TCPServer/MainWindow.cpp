#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <thread>
#include <qtimer.h>
#include <vld.h>

QObject* pMain;

boost::asio::io_service ios;
CAsio g_asio(ios, 1234);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	pMain = this;

	m_pScreen = new Screen(this);
	m_pCmd = new Cmd(this);
	m_pProcess = new Process(this);

	//心跳检测
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(HeartBeat()));
	timer->start(30000);//1000毫秒(单位)

	//首先创建一个连接的线程
    //std::thread(&CAsio::Accept, &g_asio).detach();
	std::thread asioThread([&]() {
		g_asio.Accept();
		ios.run();
	});
	asioThread.detach();

	CreateMenu();
	CreateAction();
	CreateContextMenu();

	SetAttr();
	SetConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//心跳检测
void MainWindow::HeartBeat()
{
	std::thread hbeatThread([&]() {
		boost::unordered_map<int, std::shared_ptr<CSession>> handlers = g_asio.GetHandlers();
		if (handlers.empty()) { return; }

		auto  iter = handlers.begin();
		while (iter != handlers.end())
		{
			//超过3次客户端未发送心跳包视客户端为下线状态
			size_t nCount = iter->second->GetHbCount();
			if (nCount == 3)
			{
				iter->second->FreeSocket();//关闭连接

				//更新列表
				emit SendConnSig();

				break;
			}
			//累加计数
			else if (nCount < 3 && nCount >= 0)
			{
				iter->second->SetHbCount();
				++iter;
			}
		}
	});

	hbeatThread.detach();
}

void MainWindow::CreateAction()
{
	m_pActScreen = new QAction(QString::fromLocal8Bit("屏幕"), ui->tableWidget);
	m_pActFile = new QAction(QString::fromLocal8Bit("文件"), ui->tableWidget);
	m_pActCmd = new QAction(QString::fromLocal8Bit("cmd"), ui->tableWidget);
	m_pActProcess = new QAction(QString::fromLocal8Bit("进程"), ui->tableWidget);

	m_pMenu->addAction(m_pActScreen);
	m_pMenu->addAction(m_pActFile);
	m_pMenu->addAction(m_pActCmd);
	m_pMenu->addAction(m_pActProcess);
}

void MainWindow::CreateMenu()
{
	m_pMenu = new QMenu(ui->tableWidget);
}

void MainWindow::CreateContextMenu()
{
	ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::SetAttr()
{
    //设置选择行为，以行为单位
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置选择模式，选择单行
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //失去焦点
    //ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    //默许选中第一行
    //pTableWidget->selectRow(1);
    //pTableWidget->setFocus(Qt::MouseFocusReason);

    //自适应宽度
    //自适应标题头
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //自适应内容区
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::SetConnect()
{

    //更新client列表
	QObject::connect(&g_asio, SIGNAL(SendConnSig()), this, SLOT(GetConnSlot()));
	QObject::connect(this, SIGNAL(SendConnSig()), this, SLOT(GetConnSlot()));

	//右键
	QObject::connect(ui->tableWidget,
					SIGNAL(customContextMenuRequested(const QPoint &)),
					this,
					SLOT(OnCtxMenuSlot(const QPoint &)));

	// 屏幕--
	QObject::connect(m_pActScreen, 
		SIGNAL(triggered()),
		this,
		SLOT(GetScreenSlot()));
	// 屏幕--

	//文件

	//cmd--
	//右键
	QObject::connect(m_pActCmd,
		SIGNAL(triggered()),
		this,
		SLOT(GetCmdSlot()));
	//发送命令行参数
	QObject::connect(m_pCmd,
		SIGNAL(SendCmdLineSig(int, QString)),
		this,
		SLOT(SendCmdLineSlot(int, QString)));
	//cmd--

	//**************** 进程 *****************
	QObject::connect(m_pActProcess,
		SIGNAL(triggered()),
		this,
		SLOT(GetPorcessSlot()));

	QObject::connect(m_pProcess,
		SIGNAL(SendPidSig(QString)),
		this,
		SLOT(TerminatePidSlot(QString)));
	//**************** 进程 *****************
}

void MainWindow::OnCtxMenuSlot(const QPoint & pos)
{
	m_pMenu->exec(QCursor::pos());
	if (pos.isNull())
	{

	}
}

void MainWindow::GetScreenSlot()
{
	//获取选择的客户端索引
	QModelIndex index = ui->tableWidget->currentIndex();
	if (!index.isValid())
	{
		return;
	}
	QString ip = ui->tableWidget->item(index.row(), 0)->text();
	QString port = ui->tableWidget->item(index.row(), 1)->text();

	boost::unordered_map<int, std::shared_ptr<CSession>> handlers = g_asio.GetHandlers();

	if (handlers.empty()){ return; }

	auto  iter = handlers.begin();
	while (iter != handlers.end())
	{
		//匹配选择
		boost::asio::ip::tcp::endpoint remote_ep = iter->second->GetSocket().remote_endpoint();
		boost::asio::ip::address remote_ad = remote_ep.address();
		std::string s = remote_ad.to_string();

		QString ipAddr = QString::fromStdString(s);
		QString ipPort = QString::number(remote_ep.port());

		if (ip == ipAddr && port == ipPort)
		{
			//设置session
			g_asio.SendData(iter, MSG_SCREEN);
			//设置socket
			m_pScreen->SetSocket(iter->second->GetConnId());
			//显示窗口
			m_pScreen->show();
		}
		++iter;
	}
}

void MainWindow::ShowScreenSlot(const char* pData, int nlen)
{
	m_pScreen->ShowScreen(pData, nlen);
}

void MainWindow::GetCmdSlot()
{
	//获取选择的客户端索引
	QModelIndex index = ui->tableWidget->currentIndex();
	if (!index.isValid())
	{
		return;
	}
	QString ip = ui->tableWidget->item(index.row(), 0)->text();
	QString port = ui->tableWidget->item(index.row(), 1)->text();

	boost::unordered_map<int, std::shared_ptr<CSession>> handlers = g_asio.GetHandlers();
	if (handlers.empty()) { return; }

	auto  iter = handlers.begin();
	while (iter != handlers.end())
	{
		//匹配选择
		boost::asio::ip::tcp::endpoint remote_ep = iter->second->GetSocket().remote_endpoint();
		boost::asio::ip::address remote_ad = remote_ep.address();
		std::string s = remote_ad.to_string();

		QString ipAddr = QString::fromStdString(s);
		QString ipPort = QString::number(remote_ep.port());

		if (ip == ipAddr && port == ipPort)
		{
			//设置session
			//g_asio.SendData(iter, MSG_CMD);
			iter->second->Write(MSG_CMD_YES);
			//设置socket
			m_pCmd->SetSocket(iter->second->GetConnId());
			//显示窗口
			m_pCmd->show();
		}
		++iter;
	}
}

void MainWindow::ShowCmdSlot(QByteArray baData)
{
	m_pCmd->ShowData(baData);
}

void MainWindow::SendCmdLineSlot(int connId, QString sline)
{
	//遍历client集合
	boost::unordered_map<int, std::shared_ptr<CSession>> handlers = g_asio.GetHandlers();
	if (handlers.empty()) { return; }

	auto  iter = handlers.begin();
	while (iter != handlers.end())
	{
		if (iter->second->GetConnId() == connId)
		{
			//设置session,这个write带数据
			iter->second->Write(MSG_CMD, sline);
		}
		++iter;//向后移动
	}
}

void MainWindow::CloseScreenSlot()
{
	//遍历client集合
	boost::unordered_map<int, std::shared_ptr<CSession>> handlers = g_asio.GetHandlers();
	if (handlers.empty()) { return; }

	auto  iter = handlers.begin();
	while (iter != handlers.end())
	{
		if (iter->second->GetConnId() == m_pScreen->m_nconnId)
		{
			//设置session,这个write带数据
			iter->second->Write(MSG_SCREEN_CLOSE);
		}
		++iter;//向后移动
	}
}

void MainWindow::GetPorcessSlot()
{
	QModelIndex index = ui->tableWidget->currentIndex();
	if (!index.isValid()){ return; }

	boost::unordered_map<int, std::shared_ptr<CSession>> handlers = g_asio.GetHandlers();
	if (handlers.empty()) { return; }

	QString ip = ui->tableWidget->item(index.row(), 0)->text();
	QString port = ui->tableWidget->item(index.row(), 1)->text();

	auto  iter = handlers.begin();
	while (iter != handlers.end())
	{
		boost::asio::ip::tcp::endpoint remote_ep = iter->second->GetSocket().remote_endpoint();
		boost::asio::ip::address remote_ad = remote_ep.address();
		std::string s = remote_ad.to_string();

		QString ipAddr = QString::fromStdString(s);
		QString ipPort = QString::number(remote_ep.port());

		if (ip == ipAddr && port == ipPort)
		{
			iter->second->Write(MSG_PROCESS);
			//设置socket
			m_pProcess->SetSocket(iter->second->GetConnId());
			//显示窗口
			m_pProcess->show();
		}
		++iter;
	}
}

void MainWindow::ShowProcessSlot(QByteArray baBuf)
{
	m_pProcess->ShowProcess(baBuf);
}

void MainWindow::TerminatePidSlot(QString pid)
{
	boost::unordered_map<int, std::shared_ptr<CSession>> handlers = g_asio.GetHandlers();
	if (handlers.empty()) { return; }

	auto  iter = handlers.begin();
	while (iter != handlers.end())
	{
		if (iter->second->GetConnId() == m_pProcess->GetSocket())
		{
			//设置session,这个write带数据
			iter->second->Write(MSG_PROCESS_TERMINATE, pid);
		}
		++iter;//向后移动
	}
}

void MainWindow::GetConnSlot()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();

    int i = 0;

    //遍历client集合
	boost::unordered_map<int, std::shared_ptr<CSession>> handlers = g_asio.GetHandlers();

	if (handlers.empty()) { return; }

    auto  iter = handlers.begin();
    while (iter != handlers.end())
    {
		boost::asio::ip::tcp::endpoint remote_ep = iter->second->GetSocket().remote_endpoint();
		boost::asio::ip::address remote_ad = remote_ep.address();
		std::string s = remote_ad.to_string();
		
        QString ipAddr = QString::fromStdString(s);
        QString ipPort = QString::number(remote_ep.port());

        //产生新行
        ui->tableWidget->insertRow(i);
        //IP地址
        QTableWidgetItem* item1 = new QTableWidgetItem(ipAddr);
        ui->tableWidget->setItem(i, 0, item1);
        //端口号
        QTableWidgetItem* item2 = new QTableWidgetItem(ipPort);
        ui->tableWidget->setItem(i, 1, item2);
        i++;

        ++iter;//向后移动
    }
}