#ifndef SPYPPTAB_H
#define SPYPPTAB_H

#include "FullScreenLayer.h"

#include <QWidget>
#include <qt_windows.h>

namespace Ui {
class SpyppTab;
}

class SpyppTab : public QWidget
{
    Q_OBJECT

public:
    explicit SpyppTab(QWidget *parent = nullptr);
    ~SpyppTab();

    //set style
    void setStyle();

    //set connect
    void setConnect();

    //set data
    void setData(HWND hwnd);

    //painter window
    void paintWnd(HWND hwnd);

    void linerect(HWND &hWnd,HDC &hDC,HBRUSH &NewBrush,RECT &r,PAINTSTRUCT &Ps);//线框矩形

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
//    void paintEvent(QPaintEvent *event);

private:
    Ui::SpyppTab *ui;

    bool hwndFlag = false;

    QSharedPointer<FullScreenLayer>  m_pFSLayer;

signals:

public slots:

private slots:
    void on_pushButton_pressed();
    void on_pushButton_clicked();
};

#endif // SPYPPTAB_H
