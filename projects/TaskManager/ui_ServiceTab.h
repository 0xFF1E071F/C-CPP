/********************************************************************************
** Form generated from reading UI file 'ServiceTab.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICETAB_H
#define UI_SERVICETAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServiceTab
{
public:
    QTableWidget *tableWidget;

    void setupUi(QWidget *ServiceTab)
    {
        if (ServiceTab->objectName().isEmpty())
            ServiceTab->setObjectName(QString::fromUtf8("ServiceTab"));
        ServiceTab->resize(459, 424);
        tableWidget = new QTableWidget(ServiceTab);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 10, 441, 411));

        retranslateUi(ServiceTab);

        QMetaObject::connectSlotsByName(ServiceTab);
    } // setupUi

    void retranslateUi(QWidget *ServiceTab)
    {
        ServiceTab->setWindowTitle(QApplication::translate("ServiceTab", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ServiceTab", "\345\220\215\347\247\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ServiceTab", "\346\217\217\350\277\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ServiceTab", "\347\212\266\346\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServiceTab: public Ui_ServiceTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICETAB_H
