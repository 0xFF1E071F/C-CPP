/********************************************************************************
** Form generated from reading UI file 'RegEditTab.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGEDITTAB_H
#define UI_REGEDITTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegEditTab
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QTableWidget *tableWidget;

    void setupUi(QWidget *RegEditTab)
    {
        if (RegEditTab->objectName().isEmpty())
            RegEditTab->setObjectName(QString::fromUtf8("RegEditTab"));
        RegEditTab->resize(800, 600);
        RegEditTab->setMinimumSize(QSize(800, 600));
        layoutWidget = new QWidget(RegEditTab);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(12, 12, 781, 581));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeView = new QTreeView(layoutWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setMinimumSize(QSize(0, 0));
        treeView->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(treeView);

        tableWidget = new QTableWidget(layoutWidget);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setMinimumSize(QSize(0, 0));
        tableWidget->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(tableWidget);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 3);

        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(RegEditTab);

        QMetaObject::connectSlotsByName(RegEditTab);
    } // setupUi

    void retranslateUi(QWidget *RegEditTab)
    {
        RegEditTab->setWindowTitle(QApplication::translate("RegEditTab", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("RegEditTab", "\345\220\215\347\247\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("RegEditTab", "\347\261\273\345\236\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("RegEditTab", "\346\225\260\346\215\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegEditTab: public Ui_RegEditTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGEDITTAB_H
