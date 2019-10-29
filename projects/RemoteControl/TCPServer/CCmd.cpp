#include "CCmd.h"
#include "ui_CCmd.h"

Cmd::Cmd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cmd)
{
    ui->setupUi(this);

	//发送请求
	QObject::connect(ui->pushButton,
		SIGNAL(clicked()),
		this,
		SLOT(SendDataSlot()));
}

Cmd::~Cmd()
{
    delete ui;
}

void Cmd::ShowData(QByteArray baData)
{
	ui->textBrowser->append(QString::fromLocal8Bit(baData.data()));
}

void Cmd::SendDataSlot()
{
	QString line = ui->lineEdit->text();
	emit SendCmdLineSig(m_nconnId, line);
}
