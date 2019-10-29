#ifndef FULLSCREENLAYER_H
#define FULLSCREENLAYER_H

#include <QWidget>

namespace Ui {
class FullScreenLayer;
}

class FullScreenLayer : public QWidget
{
    Q_OBJECT

public:
    explicit FullScreenLayer(QWidget *parent = nullptr);
    ~FullScreenLayer();

    //set rect
    void setRect(int x, int y, int width, int height);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::FullScreenLayer *ui;

    QRect m_nRect;
};

#endif // FULLSCREENLAYER_H
