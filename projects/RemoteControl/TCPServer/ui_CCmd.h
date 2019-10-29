/********************************************************************************
** Form generated from reading UI file 'CCmd.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CCMD_H
#define UI_CCMD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Cmd
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTextBrowser *textBrowser;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Cmd)
    {
        if (Cmd->objectName().isEmpty())
            Cmd->setObjectName(QString::fromUtf8("Cmd"));
        Cmd->resize(470, 298);
        Cmd->setMinimumSize(QSize(0, 0));
        centralwidget = new QWidget(Cmd);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 0, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        Cmd->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Cmd);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 470, 23));
        Cmd->setMenuBar(menubar);
        statusbar = new QStatusBar(Cmd);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Cmd->setStatusBar(statusbar);

        retranslateUi(Cmd);

        QMetaObject::connectSlotsByName(Cmd);
    } // setupUi

    void retranslateUi(QMainWindow *Cmd)
    {
        Cmd->setWindowTitle(QApplication::translate("Cmd", "CMD", nullptr));
        label->setText(QApplication::translate("Cmd", "\350\276\223\345\207\272:", nullptr));
        label_2->setText(QApplication::translate("Cmd", "\345\221\275\344\273\244:", nullptr));
        pushButton->setText(QApplication::translate("Cmd", "\350\257\267\346\261\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cmd: public Ui_Cmd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CCMD_H
