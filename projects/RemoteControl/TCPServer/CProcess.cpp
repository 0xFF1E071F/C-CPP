#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <vector>
#include <sstream>
#include "CProcess.h"
#include "ui_CProcess.h"

using namespace boost::property_tree;

Process::Process(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Process)
{
    ui->setupUi(this);

	CreateMenu();
	CreateAction();
	CreateContextMenu();

	QObject::connect(ui->tableWidget,
		SIGNAL(customContextMenuRequested(const QPoint &)),
		this,
		SLOT(ProcessOnCtxMenuSlot(const QPoint &)));

	QObject::connect(m_pActTerminate,
		SIGNAL(triggered()),
		this,
		SLOT(ProcessTerminateSlot()));
}

Process::~Process()
{
    delete ui;
}


void Process::CreateMenu()
{
	m_pMenu = new QMenu(ui->tableWidget);
}

void Process::CreateAction()
{
	m_pActTerminate = new QAction(QString::fromLocal8Bit("结束进程"), ui->tableWidget);
	m_pMenu->addAction(m_pActTerminate);
}

void Process::CreateContextMenu()
{
	ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

void Process::ProcessTerminateSlot()
{
	QModelIndex index = ui->tableWidget->currentIndex();
	if (!index.isValid()) { return; }
	QString pid = ui->tableWidget->item(index.row(), 1)->text();
	emit SendPidSig(pid);
}

void Process::ProcessOnCtxMenuSlot(const QPoint &)
{
	m_pMenu->exec(QCursor::pos());
}

void Process::ShowProcess(QByteArray baBuf)
{
	try
	{
		wptree pt;

		//QByteArray to qstring
		QString qstr = QString(baBuf.data());
		//qstring to wstring
		std::wstring wstr = qstr.toStdWString();
		//wstringstream construct with wstring
		std::wstringstream wstream(wstr);

		read_json(wstream, pt);

		wptree aryProcess = pt.get_child(L"aryProcess");

		int i = 0;

		BOOST_FOREACH(wptree::value_type &v, aryProcess)
		{
			//QString pName = QString::fromStdWString(v.first);
			QString pName = QString::fromStdWString(v.second.get<std::wstring>(L"Name"));
			QString pId = QString::fromStdWString(v.second.get<std::wstring>(L"ID"));
			//qDebug() << "pName: " << pName << "  " << "pId: " << pId << '\n';

			ui->tableWidget->insertRow(i);

			//进程名
			QTableWidgetItem* item1 = new QTableWidgetItem(pName);
			ui->tableWidget->setItem(i, 0, item1);

			//进程ID
			QTableWidgetItem* item2 = new QTableWidgetItem(pId);
			ui->tableWidget->setItem(i, 1, item2);

			i++;
		}
	}
	catch (ptree_error pterr)
	{
		pterr.what();
		return;
	}
}
