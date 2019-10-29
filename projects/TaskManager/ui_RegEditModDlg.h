/********************************************************************************
** Form generated from reading UI file 'RegEditModDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGEDITMODDLG_H
#define UI_REGEDITMODDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegEditModDlg
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *lineEdit_key;
    QLabel *label_2;
    QLineEdit *lineEdit_data;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *RegEditModDlg)
    {
        if (RegEditModDlg->objectName().isEmpty())
            RegEditModDlg->setObjectName(QString::fromUtf8("RegEditModDlg"));
        RegEditModDlg->resize(450, 210);
        RegEditModDlg->setMinimumSize(QSize(450, 210));
        RegEditModDlg->setMaximumSize(QSize(450, 210));
        widget = new QWidget(RegEditModDlg);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 431, 191));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        lineEdit_key = new QLineEdit(widget);
        lineEdit_key->setObjectName(QString::fromUtf8("lineEdit_key"));
        lineEdit_key->setEnabled(false);
        lineEdit_key->setMaximumSize(QSize(16777215, 30));

        verticalLayout->addWidget(lineEdit_key);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        lineEdit_data = new QLineEdit(widget);
        lineEdit_data->setObjectName(QString::fromUtf8("lineEdit_data"));
        lineEdit_data->setMaximumSize(QSize(16777215, 30));

        verticalLayout->addWidget(lineEdit_data);


        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(widget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setMaximumSize(QSize(16777215, 40));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(RegEditModDlg);

        QMetaObject::connectSlotsByName(RegEditModDlg);
    } // setupUi

    void retranslateUi(QDialog *RegEditModDlg)
    {
        RegEditModDlg->setWindowTitle(QApplication::translate("RegEditModDlg", "Dialog", nullptr));
        label->setText(QApplication::translate("RegEditModDlg", "\346\225\260\345\200\274\345\220\215\347\247\260(N):", nullptr));
        label_2->setText(QApplication::translate("RegEditModDlg", "\346\225\260\345\200\274\346\225\260\346\215\256(V):", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegEditModDlg: public Ui_RegEditModDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGEDITMODDLG_H
