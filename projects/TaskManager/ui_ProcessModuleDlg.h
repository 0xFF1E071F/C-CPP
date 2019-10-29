/********************************************************************************
** Form generated from reading UI file 'ProcessModuleDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROCESSMODULEDLG_H
#define UI_PROCESSMODULEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ProcessModuleDlg
{
public:
    QTableWidget *tableWidget;

    void setupUi(QDialog *ProcessModuleDlg)
    {
        if (ProcessModuleDlg->objectName().isEmpty())
            ProcessModuleDlg->setObjectName(QString::fromUtf8("ProcessModuleDlg"));
        ProcessModuleDlg->resize(617, 409);
        tableWidget = new QTableWidget(ProcessModuleDlg);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 20, 601, 381));

        retranslateUi(ProcessModuleDlg);

        QMetaObject::connectSlotsByName(ProcessModuleDlg);
    } // setupUi

    void retranslateUi(QDialog *ProcessModuleDlg)
    {
        ProcessModuleDlg->setWindowTitle(QApplication::translate("ProcessModuleDlg", "Dialog", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ProcessModuleDlg", "\346\250\241\345\235\227", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ProcessModuleDlg", "\350\267\257\345\276\204", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProcessModuleDlg: public Ui_ProcessModuleDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESSMODULEDLG_H
