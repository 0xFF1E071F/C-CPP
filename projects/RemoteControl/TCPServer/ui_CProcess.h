/********************************************************************************
** Form generated from reading UI file 'CProcess.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CPROCESS_H
#define UI_CPROCESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Process
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Process)
    {
        if (Process->objectName().isEmpty())
            Process->setObjectName(QString::fromUtf8("Process"));
        Process->resize(383, 513);
        centralwidget = new QWidget(Process);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(centralwidget);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);

        Process->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Process);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 383, 23));
        Process->setMenuBar(menubar);
        statusbar = new QStatusBar(Process);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Process->setStatusBar(statusbar);

        retranslateUi(Process);

        QMetaObject::connectSlotsByName(Process);
    } // setupUi

    void retranslateUi(QMainWindow *Process)
    {
        Process->setWindowTitle(QApplication::translate("Process", "\350\277\233\347\250\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Process", "\350\277\233\347\250\213\345\220\215", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Process", "\350\277\233\347\250\213ID", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Process: public Ui_Process {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CPROCESS_H
