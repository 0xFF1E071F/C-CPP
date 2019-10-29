#include <qpainter.h>
#include <qscreen.h>
#include "CScreen.h"
#include "ui_CScreen.h"
#include "../common/common.h"

const int MAX_CAPTURE_NUM = 256;

Screen::Screen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Screen)
{
    ui->setupUi(this);

	//通知客户端不用再发送数据了
	QObject::connect(this,
		SIGNAL(CloseSendSig()),
		parent,
		SLOT(CloseScreenSlot()));

	m_nCount = 1;
}

Screen::~Screen()
{
    delete ui;
}

bool Screen::ShowScreen(const char * ptr, int nlen)
{
	//解压：收到数据，读包之后，解压
	//BYTE *pUncompressData = NULL;     //解压后数据
	//DWORD dwUncompressDataLength = 0; //解压后数据长度
	//UncompressData((BYTE*)ptr, nlen, &pUncompressData, &dwUncompressDataLength);


	//width heigth
	width = *(int*)ptr;
	height = *((int*)ptr + 1);
	//width = *(int*)pUncompressData;
	//height = *((int*)pUncompressData + 1);

	//data
	char *pData = (char*)ptr + 8;
	//char* pData = (char*)pUncompressData + 8;
	//length
	//uint nlength = nlen - 8;

	m_image = QImage((const uchar*)pData, width, height, QImage::Format_ARGB32);
	m_deepcopyImg = m_image.copy();

	//释放堆
	char *freep = (pData - 8);
	if (freep)
	{
		delete[]freep;
		freep = nullptr;
	}

	//if (pUncompressData)
	//{
	//	delete[]pUncompressData;
	//	pUncompressData = NULL;
	//}

	this->update();

	return true;
}

void Screen::paintEvent(QPaintEvent * event)
{
	if (!m_deepcopyImg.isNull())
	{
		QPainter painter(this);
		painter.drawImage(0, 0, m_deepcopyImg);

		if (event)
		{
			return;
		}
	}
}

void Screen::mouseDoubleClickEvent(QMouseEvent *)
{
	if (m_nCount > MAX_CAPTURE_NUM)
	{
		return;
	}

	QScreen* screen = QGuiApplication::primaryScreen();
	QString fileName = QString("dskcapture_%1.bmp").arg(m_nCount);
	screen->grabWindow(this->winId()).save(fileName, "bmp");
	++m_nCount;
}

void Screen::closeEvent(QCloseEvent * event)
{
	//QMessageBox::information(this, "tooltip", QString::fromLocal8Bit("你是否要关闭?"), QMessageBox::Yes | QMessageBox::No);
	
}
