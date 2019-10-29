#include "FullScreenLayer.h"
#include "ui_FullScreenLayer.h"

#include <qpainter.h>
#include <qdebug.h>

FullScreenLayer::FullScreenLayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FullScreenLayer)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Tool |
                         Qt::FramelessWindowHint|
                         Qt::WindowStaysOnTopHint
                         );

    this->setWindowOpacity(0.8);//设置窗口透明度
//    this->setWindowOpacity(1);//设置窗口透明度

    this->setAttribute(Qt::WA_TranslucentBackground, true);

//    QPalette pal(this->palette());
//    pal.setColor(QPalette::Background, Qt::black); //设置背景黑色
//    this->setAutoFillBackground(true);
//    this->setPalette(pal);
    this->showFullScreen();
}

FullScreenLayer::~FullScreenLayer()
{
    delete ui;
}

void FullScreenLayer::setRect(int x, int y, int width, int height)
{
    qDebug() << "setRect" << "x " << x
             << "y " << y
             << "width " << width
             << "height " << height <<'\n';
    m_nRect = QRect(x, y, width, height);
//    paintWnd();

    this->update();
}

void FullScreenLayer::paintEvent(QPaintEvent *event)
{
    if(m_nRect.isValid())
    {
        qDebug() << "paintEvent" << '\n';
        QPainter p(this);

        p.setPen(QPen(Qt::red, 3));
        p.drawRect(m_nRect);
        // 绘制图标
        //QRect tmpr = QRect(m_nRect.x() + 10, m_nRect.y(), m_nRect.width(), m_nRect.height());
        //p.drawPixmap(tmpr, QPixmap(":/myimages/images/dog.png"));

        qDebug() << "paintEvent_2" << '\n';
    }
}
