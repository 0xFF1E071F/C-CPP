#ifndef SCREEN_H
#define SCREEN_H

#include <QMainWindow>
#include <qpixmap.h>
#include <qimage.h>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class Screen;
}

class Screen : public QMainWindow
{
    Q_OBJECT

public:
    explicit Screen(QWidget *parent = nullptr);
    ~Screen();

	bool ShowScreen(const char* ptr, int nlen);
	void SetSocket(int connId)
	{
		m_nconnId = connId;
	}

protected:
	//��ͼ
	void paintEvent(QPaintEvent *event);
	//˫����ͼ
	void mouseDoubleClickEvent(QMouseEvent*);
	//����һ���麯�����̳���QEvent.ֻҪ��д������麯�������㰴�´������Ͻǵ�"��"ʱ���ͻ����������д�Ĵ˺���.
	void closeEvent(QCloseEvent*event);


public:
	QImage m_image;
	QImage m_deepcopyImg;
	QPixmap m_pixmap;
	int width;
	int height;

	int m_nCount;

	size_t m_nconnId;

private:
    Ui::Screen *ui;

	
};

#endif // SCREEN_H
