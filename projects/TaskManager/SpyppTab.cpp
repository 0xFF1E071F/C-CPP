#include "SpyppTab.h"
#include "ui_SpyppTab.h"
#include <QPainter>
#include <qdebug.h>
#include <qt_windows.h>

SpyppTab::SpyppTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpyppTab)
{
    ui->setupUi(this);

    setStyle();
}

SpyppTab::~SpyppTab()
{
    delete ui;
}

void SpyppTab::setStyle()
{
    //findwnd
    ui->pushButton->setIcon(QIcon(":/myimages/images/findwnd.png")); //设置背景图片的来源
    ui->pushButton->setIconSize(QSize(ui->pushButton->width(),ui->pushButton->height())); //设置填充按键这么大的
    ui->pushButton->setStyleSheet("border-style:flat");  //设置透明的
}

void SpyppTab::setConnect()
{

}

void SpyppTab::setData(HWND hwnd)
{
    TCHAR szTitle[MAX_PATH] = { 0 };
    TCHAR szClass[MAX_PATH] = { 0 };
    DWORD dwProcessId = 0;
    int nMaxCount = MAX_PATH;

    LPTSTR  lpWindowName = szTitle;
    LPTSTR  lpClassName = szClass;

    GetWindowText(hwnd, lpWindowName, nMaxCount);
    GetClassName(hwnd, lpClassName, nMaxCount);
    //hwnd
    QString strHwnd = QString(QString::fromLocal8Bit("%1")).
            arg(QString::number(reinterpret_cast<std::uintptr_t>(hwnd),16), 8, QLatin1Char('0'));
    ui->lineEdit_handle->setText(strHwnd.toUpper());
    //title
    QString strTitle = QString(QString::fromLocal8Bit("%1")).
            arg(QString::fromWCharArray(lpWindowName));
    ui->lineEdit_title->setText(strTitle);
    //class
    QString strClass = QString(QString::fromLocal8Bit("%1")).
            arg(QString::fromWCharArray(lpClassName));
    ui->lineEdit_class->setText(strClass);
}

void SpyppTab::paintWnd(HWND hwnd)
{
    //windows api
    RECT rect;

    GetWindowRect(hwnd, &rect);

    //QT
    int width = rect.right - rect.left;
    int height = rect.bottom -  rect.top;
    m_pFSLayer->setRect(rect.left, rect.top, width, height);

    update();

//
//    QPoint topLeft = QPoint(rect.top, rect.left);
//    qDebug() << "topLeft " << topLeft << '\n';
//    QPoint bottomRight = QPoint(rect.bottom, rect.right);
//    qDebug() << "bottomRight " << bottomRight << '\n';
//    QRect qrect = QRect(topLeft, bottomRight);
//    qDebug() << "qrect " << qrect << '\n';

//    QPainter p;
//    p.setPen(QColor(RGB(255, 0, 0)));
//    p.drawRect(qrect);


//    HDC         hDC;
//    PAINTSTRUCT Ps;
//    HBRUSH      NewBrush;
//    RECT	r;

//    linerect(hwnd,hDC,NewBrush,r,Ps);
}

void SpyppTab::linerect(HWND &hWnd, HDC &hDC, HBRUSH &NewBrush, RECT &r, PAINTSTRUCT &Ps)
{

////    HWND hWnd = (HWND)5244612;  // 作为场景窗口句柄
//        HDC nDc= GetWindowDC(hWnd);
//        SetROP2(nDc,R2_NOTXORPEN);
//        RECT rect;
//        GetWindowRect(hWnd,&rect); //画场景句柄矩形 所以left 和 top 都可以设置为0;
//        rect.left = 0;
//        rect.top = 0;
//        HPEN nHpen = CreatePen(0,3,RGB(0,0,255));
//        HGDIOBJ nHg =  SelectObject(nDc,nHpen);
//        Rectangle(nDc,rect.left+10,rect.top+10,rect.bottom,rect.right-50);//
//        //    上面的加减是为了更画 窗口矩形的位置准确所运算的
//        SelectObject(nDc,nHg);
//        DeleteObject(nHpen);
//        ReleaseDC(hWnd,nDc);
//    //-----------------------
    hDC=GetDC(hWnd);
    qDebug() << "hDC" << hDC << '\n';

    NewBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hDC, NewBrush);

//    SetRect (&r, 250, 250,400, 400);
    GetWindowRect(hWnd, &r);

    //FrameRect(hDC, &r, NewBrush);
    Rectangle(hDC,r.left+10,r.top+10,r.bottom,r.right-50);

    DeleteObject(NewBrush);
    DeleteObject(NewBrush);

    ReleaseDC(hWnd,hDC);
}

void SpyppTab::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug() << "mouseMoveEvent" << '\n';
    //get cursor pos
//    qDebug() << QCursor::pos() << '\n';

    qDebug() << "hwndFlag" << hwndFlag << '\n';
    if(hwndFlag)
    {
        //window api
        POINT pos;
        GetCursorPos(&pos);
        //get window handle
        HWND hwndPointNow = WindowFromPoint(pos);

        qDebug() << hwndPointNow << '\n';

        //set data to ui
        //...
//        if(IsWindow(hwndPointNow) && IsWindowVisible(hwndPointNow))
//        if(IsWindow(hwndPointNow))
//        {
            //painter red
            paintWnd(hwndPointNow);

            setData(hwndPointNow);
//        }

        //recovery hwnd flag
        //hwndFlag = false;
    }
}

void SpyppTab::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouseReleaseEvent" << '\n';
    //
    //change button image, button click
    ui->pushButton->setIcon(QIcon(":/myimages/images/findwnd.png")); //设置背景图片的来源
    ui->pushButton->setIconSize(QSize(ui->pushButton->width(),ui->pushButton->height())); //设置填充按键这么大的
//    ui->toolButton->setStyleSheet("border-style:flat");  //设置透明的

    //recovery hwnd flag
    hwndFlag = false;

    m_pFSLayer->setRect(0, 0, 0, 0);

    ReleaseCapture();
    //set cursor
    setCursor(Qt::ArrowCursor);
}

//void SpyppTab::paintEvent(QPaintEvent *event)
//{
//    qDebug() << "paintEvent(QPaintEvent *event)" << '\n';
//    QPainter painter(this);
//    painter.drawLine(80, 100, 650, 500);
//    painter.setPen(Qt::red);
//    painter.drawRect(10, 10, 100, 400);
//    painter.setPen(QPen(Qt::green, 5));
//    painter.setBrush(Qt::blue);
//    painter.drawEllipse(50, 150, 400, 200);
//}

void SpyppTab::on_pushButton_pressed()
{
    m_pFSLayer = QSharedPointer<FullScreenLayer>( new FullScreenLayer () );

    qDebug() << "on_pushButton_pressed" << '\n';
    //
    //change button image, button click
    ui->pushButton->setIcon(QIcon(":/myimages/images/btnclick.png")); //设置背景图片的来源
    ui->pushButton->setIconSize(QSize(ui->pushButton->width(),ui->pushButton->height())); //设置填充按键这么大的
//    ui->pushButton->setStyleSheet("border-style:flat");  //设置透明的

    //set get hwnd flag
    hwndFlag = true;

    //set cursor
    QCursor cursor;
    //加载图片
    QPixmap pixmap(":/myimages/images/mouse.png");
    //定义大小
    QSize picSize(20, 20);
    //缩放图片，按比例
    QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::KeepAspectRatio);
    //设置样式
    cursor = QCursor(scaledPixmap, -1, -1);
    //更改鼠标形状
    setCursor(cursor);
}

void SpyppTab::on_pushButton_clicked()
{
    qDebug() << "on_toolButton_clicked" << '\n';
    //
    //change button image, button click
    ui->pushButton->setIcon(QIcon(":/myimages/images/findwnd.png")); //设置背景图片的来源
    ui->pushButton->setIconSize(QSize(ui->pushButton->width(),ui->pushButton->height())); //设置填充按键这么大的
//    ui->toolButton->setStyleSheet("border-style:flat");  //设置透明的

    //recovery hwnd flag
    hwndFlag = false;

    m_pFSLayer->setRect(0, 0, 0, 0);

    ReleaseCapture();

    //set cursor
    setCursor(Qt::ArrowCursor);
}
