#ifndef REGEDITMODDLG_H
#define REGEDITMODDLG_H

#include <QDialog>
#include <qtablewidget.h>

namespace Ui {
class RegEditModDlg;
}

class RegEditModDlg : public QDialog
{
    Q_OBJECT

public:
    explicit RegEditModDlg(QWidget *parent = nullptr);
    ~RegEditModDlg();

    //connect
    void setConnect();

    //set ui style
    void setStyle();

    //insert data to dialog
    void setData(QString key, QString val);

private:
    Ui::RegEditModDlg *ui;

    //send Data param,
    //true ==> add new regedit key-value
    //false ==> don't add, just modify regedit value
    bool sendFlag = true;

signals:
    //send data to parent window
    void sendData(QString str);
    //send modify flag to parent window
    void sendModDataSig(QString str);

public slots:
    void onButtonBoxOk();
    void onButtonBoxCancel();
};

#endif // REGEDITMODDLG_H
