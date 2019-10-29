/********************************************************************************
** Form generated from reading UI file 'ProcessThreadDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROCESSTHREADDLG_H
#define UI_PROCESSTHREADDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ProcessThreadDlg
{
public:
    QTableWidget *tableWidget;

    void setupUi(QDialog *ProcessThreadDlg)
    {
        if (ProcessThreadDlg->objectName().isEmpty())
            ProcessThreadDlg->setObjectName(QString::fromUtf8("ProcessThreadDlg"));
        ProcessThreadDlg->resize(576, 375);
        tableWidget = new QTableWidget(ProcessThreadDlg);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 10, 561, 351));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);

        retranslateUi(ProcessThreadDlg);

        QMetaObject::connectSlotsByName(ProcessThreadDlg);
    } // setupUi

    void retranslateUi(QDialog *ProcessThreadDlg)
    {
        ProcessThreadDlg->setWindowTitle(QApplication::translate("ProcessThreadDlg", "Dialog", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ProcessThreadDlg", "\347\272\277\347\250\213Id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ProcessThreadDlg", "\344\274\230\345\205\210\347\272\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProcessThreadDlg: public Ui_ProcessThreadDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESSTHREADDLG_H
