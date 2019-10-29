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
	//绘图
	void paintEvent(QPaintEvent *event);
	//双击截图
	void mouseDoubleClickEvent(QMouseEvent*);
	//这是一个虚函数，继承自QEvent.只要重写了这个虚函数，当你按下窗口右上角的"×"时，就会调用你所重写的此函数.
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
