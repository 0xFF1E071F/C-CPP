/********************************************************************************
** Form generated from reading UI file 'TraversalWndTab.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAVERSALWNDTAB_H
#define UI_TRAVERSALWNDTAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TraversalWndTab
{
public:
    QTreeView *treeView;

    void setupUi(QWidget *TraversalWndTab)
    {
        if (TraversalWndTab->objectName().isEmpty())
            TraversalWndTab->setObjectName(QString::fromUtf8("TraversalWndTab"));
        TraversalWndTab->resize(400, 300);
        treeView = new QTreeView(TraversalWndTab);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setGeometry(QRect(10, 10, 381, 281));

        retranslateUi(TraversalWndTab);

        QMetaObject::connectSlotsByName(TraversalWndTab);
    } // setupUi

    void retranslateUi(QWidget *TraversalWndTab)
    {
        TraversalWndTab->setWindowTitle(QApplication::translate("TraversalWndTab", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TraversalWndTab: public Ui_TraversalWndTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAVERSALWNDTAB_H
