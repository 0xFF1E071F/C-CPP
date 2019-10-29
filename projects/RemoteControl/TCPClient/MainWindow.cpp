#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ctcpsocket.h"
#include "../common/common.h"
#include <thread>
#include <qtimer.h>

CTcpSocket g_Client;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	Start();
}

//心跳发送
void MainWindow::HeartBeat()
{
	std::thread heartbeatThread([&]() {
		SendData(g_Client.m_ConnectSocket, MSG_HEARTBEAT, nullptr, 0);
	});

	heartbeatThread.detach();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::Start()
{
	g_Client.StartUp();
	g_Client.CreateAndConnect(1234);
	g_Client.RecvThread();

	//定时器，检测客户端连接情况
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(HeartBeat()));
	timer->start(10000);//1000毫秒(单位)

	return true;
}