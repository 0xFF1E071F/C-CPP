/********************************************************************************
** Form generated from reading UI file 'FullScreenLayer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FULLSCREENLAYER_H
#define UI_FULLSCREENLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FullScreenLayer
{
public:

    void setupUi(QWidget *FullScreenLayer)
    {
        if (FullScreenLayer->objectName().isEmpty())
            FullScreenLayer->setObjectName(QString::fromUtf8("FullScreenLayer"));
        FullScreenLayer->resize(400, 300);

        retranslateUi(FullScreenLayer);

        QMetaObject::connectSlotsByName(FullScreenLayer);
    } // setupUi

    void retranslateUi(QWidget *FullScreenLayer)
    {
        FullScreenLayer->setWindowTitle(QApplication::translate("FullScreenLayer", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FullScreenLayer: public Ui_FullScreenLayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FULLSCREENLAYER_H
