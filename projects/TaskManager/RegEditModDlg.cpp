#include "RegEditModDlg.h"
#include "ui_RegEditModDlg.h"
#include <qdebug.h>
#include <qpushbutton.h>

RegEditModDlg::RegEditModDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegEditModDlg)
{
    ui->setupUi(this);

    setStyle();
    setConnect();
}

RegEditModDlg::~RegEditModDlg()
{
    delete ui;
}

void RegEditModDlg::setConnect()
{
//    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
//    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onButtonBoxOk()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(onButtonBoxCancel()));
}

void RegEditModDlg::setStyle()
{
    //lineEdit_key
    setWindowTitle(QString::fromLocal8Bit("编辑字符串"));
    ui->lineEdit_key->setReadOnly(false);
    //--lineEdit_key

    //buttonBox
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QString::fromLocal8Bit("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QString::fromLocal8Bit("取消"));
    //--buttonBox
}

void RegEditModDlg::setData(QString key, QString val)
{
    sendFlag = false;

    //set key to dialog
    ui->lineEdit_key->setText(key);

    //set val to dialog
    ui->lineEdit_data->setText(val);
}

void RegEditModDlg::onButtonBoxOk()
{
    //qDebug() << "onButtonBoxOk" << '\n';
    //send signals to parent window
    if(sendFlag)
    {
        emit sendData(ui->lineEdit_data->text());
        this->close();
    }
    else {
        emit sendModDataSig(ui->lineEdit_data->text());

        this->close();

        sendFlag = true;
    }
}

void RegEditModDlg::onButtonBoxCancel()
{
    qDebug() << "onButtonBoxCancel" << '\n';
    this->close();
}
