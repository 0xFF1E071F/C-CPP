/********************************************************************************
** Form generated from reading UI file 'SpyppTab.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPYPPTAB_H
#define UI_SPYPPTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpyppTab
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *pushButton;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_title;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit_class;
    QLabel *label_2;
    QLineEdit *lineEdit_handle;

    void setupUi(QWidget *SpyppTab)
    {
        if (SpyppTab->objectName().isEmpty())
            SpyppTab->setObjectName(QString::fromUtf8("SpyppTab"));
        SpyppTab->resize(400, 300);
        layoutWidget = new QWidget(SpyppTab);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 11, 167, 52));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(50, 50));
        pushButton->setMaximumSize(QSize(50, 50));

        horizontalLayout->addWidget(pushButton);

        layoutWidget1 = new QWidget(SpyppTab);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(11, 49, 331, 201));
        gridLayout = new QGridLayout(layoutWidget1);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit_title = new QLineEdit(layoutWidget1);
        lineEdit_title->setObjectName(QString::fromUtf8("lineEdit_title"));
        lineEdit_title->setMaximumSize(QSize(16777215, 25));

        gridLayout->addWidget(lineEdit_title, 1, 1, 1, 1);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        lineEdit_class = new QLineEdit(layoutWidget1);
        lineEdit_class->setObjectName(QString::fromUtf8("lineEdit_class"));
        lineEdit_class->setMaximumSize(QSize(16777215, 25));

        gridLayout->addWidget(lineEdit_class, 2, 1, 1, 1);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        lineEdit_handle = new QLineEdit(layoutWidget1);
        lineEdit_handle->setObjectName(QString::fromUtf8("lineEdit_handle"));
        lineEdit_handle->setMaximumSize(QSize(16777215, 25));

        gridLayout->addWidget(lineEdit_handle, 0, 1, 1, 1);


        retranslateUi(SpyppTab);

        QMetaObject::connectSlotsByName(SpyppTab);
    } // setupUi

    void retranslateUi(QWidget *SpyppTab)
    {
        SpyppTab->setWindowTitle(QApplication::translate("SpyppTab", "Form", nullptr));
        label->setText(QApplication::translate("SpyppTab", "\346\237\245\346\211\276\347\250\213\345\272\217\345\267\245\345\205\267\357\274\232", nullptr));
        pushButton->setText(QApplication::translate("SpyppTab", "...", nullptr));
        label_3->setText(QApplication::translate("SpyppTab", "\346\240\207\351\242\230(C):", nullptr));
        label_4->setText(QApplication::translate("SpyppTab", "\347\261\273(L):", nullptr));
        label_2->setText(QApplication::translate("SpyppTab", "\345\217\245\346\237\204(A):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SpyppTab: public Ui_SpyppTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPYPPTAB_H
