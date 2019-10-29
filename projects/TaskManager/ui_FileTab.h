/********************************************************************************
** Form generated from reading UI file 'FileTab.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILETAB_H
#define UI_FILETAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileTab
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QLineEdit *lineEdit;

    void setupUi(QWidget *FileTab)
    {
        if (FileTab->objectName().isEmpty())
            FileTab->setObjectName(QString::fromUtf8("FileTab"));
        FileTab->setEnabled(true);
        FileTab->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FileTab->sizePolicy().hasHeightForWidth());
        FileTab->setSizePolicy(sizePolicy);
        FileTab->setMinimumSize(QSize(800, 600));
        layoutWidget = new QWidget(FileTab);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 12, 781, 581));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeView = new QTreeView(layoutWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
        treeView->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(treeView);

        horizontalLayout->setStretch(0, 1);

        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout_2->addWidget(lineEdit, 1, 0, 1, 1);


        retranslateUi(FileTab);

        QMetaObject::connectSlotsByName(FileTab);
    } // setupUi

    void retranslateUi(QWidget *FileTab)
    {
        FileTab->setWindowTitle(QApplication::translate("FileTab", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileTab: public Ui_FileTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILETAB_H
