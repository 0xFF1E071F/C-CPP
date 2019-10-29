/********************************************************************************
** Form generated from reading UI file 'CScreen.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CSCREEN_H
#define UI_CSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Screen
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Screen)
    {
        if (Screen->objectName().isEmpty())
            Screen->setObjectName(QString::fromUtf8("Screen"));
        Screen->resize(476, 391);
        centralwidget = new QWidget(Screen);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        Screen->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Screen);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 476, 23));
        Screen->setMenuBar(menubar);
        statusbar = new QStatusBar(Screen);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Screen->setStatusBar(statusbar);

        retranslateUi(Screen);

        QMetaObject::connectSlotsByName(Screen);
    } // setupUi

    void retranslateUi(QMainWindow *Screen)
    {
        Screen->setWindowTitle(QApplication::translate("Screen", "\346\241\214\351\235\242\346\210\252\345\261\217\357\274\210\345\244\207\346\263\250\357\274\232\345\217\214\345\207\273\346\210\252\345\233\276\357\274\211", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Screen: public Ui_Screen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CSCREEN_H
