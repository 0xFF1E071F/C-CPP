/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QStackedWidget *stackedWidget;
    QWidget *page_NH;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QWidget *page_SH;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tw_pesh_2;
    QTableWidget *tw_pesh_1;
    QWidget *page_Export_Directory;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_7;
    QTableWidget *tw_export_dir_1;
    QTableWidget *tw_export_dir_2;
    QWidget *page_AC;
    QFormLayout *formLayout;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QVBoxLayout *verticalLayout_6;
    QLineEdit *lineEdit_VA;
    QLineEdit *lineEdit_RVA;
    QLineEdit *lineEdit_FA;
    QWidget *page_ID;
    QGridLayout *gridLayout;
    QTableWidget *tw_peid_1;
    QTableWidget *tw_peid_2;
    QWidget *page_tls_dir;
    QGridLayout *gridLayout_5;
    QTableWidget *tw_petls;
    QWidget *page_RD;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QTableWidget *tw_perd_1;
    QTableWidget *tw_perd_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1114, 614);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeView = new QTreeView(centralWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        horizontalLayout->addWidget(treeView);

        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page_NH = new QWidget();
        page_NH->setObjectName(QString::fromUtf8("page_NH"));
        verticalLayout = new QVBoxLayout(page_NH);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableWidget = new QTableWidget(page_NH);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout->addWidget(tableWidget);

        stackedWidget->addWidget(page_NH);
        page_SH = new QWidget();
        page_SH->setObjectName(QString::fromUtf8("page_SH"));
        verticalLayout_4 = new QVBoxLayout(page_SH);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tw_pesh_2 = new QTableWidget(page_SH);
        if (tw_pesh_2->columnCount() < 10)
            tw_pesh_2->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(3, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(4, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(5, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(6, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(7, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(8, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tw_pesh_2->setHorizontalHeaderItem(9, __qtablewidgetitem13);
        if (tw_pesh_2->rowCount() < 1)
            tw_pesh_2->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tw_pesh_2->setVerticalHeaderItem(0, __qtablewidgetitem14);
        tw_pesh_2->setObjectName(QString::fromUtf8("tw_pesh_2"));

        verticalLayout_3->addWidget(tw_pesh_2);

        tw_pesh_1 = new QTableWidget(page_SH);
        if (tw_pesh_1->columnCount() < 10)
            tw_pesh_1->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(2, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(3, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(4, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(5, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(6, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(7, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(8, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tw_pesh_1->setHorizontalHeaderItem(9, __qtablewidgetitem24);
        tw_pesh_1->setObjectName(QString::fromUtf8("tw_pesh_1"));

        verticalLayout_3->addWidget(tw_pesh_1);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 4);

        verticalLayout_4->addLayout(verticalLayout_3);

        stackedWidget->addWidget(page_SH);
        page_Export_Directory = new QWidget();
        page_Export_Directory->setObjectName(QString::fromUtf8("page_Export_Directory"));
        gridLayout_2 = new QGridLayout(page_Export_Directory);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        tw_export_dir_1 = new QTableWidget(page_Export_Directory);
        if (tw_export_dir_1->columnCount() < 4)
            tw_export_dir_1->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tw_export_dir_1->setHorizontalHeaderItem(0, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tw_export_dir_1->setHorizontalHeaderItem(1, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        tw_export_dir_1->setHorizontalHeaderItem(2, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        tw_export_dir_1->setHorizontalHeaderItem(3, __qtablewidgetitem28);
        tw_export_dir_1->setObjectName(QString::fromUtf8("tw_export_dir_1"));

        verticalLayout_7->addWidget(tw_export_dir_1);

        tw_export_dir_2 = new QTableWidget(page_Export_Directory);
        if (tw_export_dir_2->columnCount() < 5)
            tw_export_dir_2->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        tw_export_dir_2->setHorizontalHeaderItem(0, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        tw_export_dir_2->setHorizontalHeaderItem(1, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        tw_export_dir_2->setHorizontalHeaderItem(2, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        tw_export_dir_2->setHorizontalHeaderItem(3, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        tw_export_dir_2->setHorizontalHeaderItem(4, __qtablewidgetitem33);
        tw_export_dir_2->setObjectName(QString::fromUtf8("tw_export_dir_2"));

        verticalLayout_7->addWidget(tw_export_dir_2);


        gridLayout_2->addLayout(verticalLayout_7, 0, 0, 1, 1);

        stackedWidget->addWidget(page_Export_Directory);
        page_AC = new QWidget();
        page_AC->setObjectName(QString::fromUtf8("page_AC"));
        formLayout = new QFormLayout(page_AC);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label = new QLabel(page_AC);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 25));

        verticalLayout_5->addWidget(label);

        label_2 = new QLabel(page_AC);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 25));

        verticalLayout_5->addWidget(label_2);

        label_3 = new QLabel(page_AC);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(0, 25));

        verticalLayout_5->addWidget(label_3);


        horizontalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        lineEdit_VA = new QLineEdit(page_AC);
        lineEdit_VA->setObjectName(QString::fromUtf8("lineEdit_VA"));
        lineEdit_VA->setMinimumSize(QSize(0, 25));

        verticalLayout_6->addWidget(lineEdit_VA);

        lineEdit_RVA = new QLineEdit(page_AC);
        lineEdit_RVA->setObjectName(QString::fromUtf8("lineEdit_RVA"));
        lineEdit_RVA->setMinimumSize(QSize(0, 25));

        verticalLayout_6->addWidget(lineEdit_RVA);

        lineEdit_FA = new QLineEdit(page_AC);
        lineEdit_FA->setObjectName(QString::fromUtf8("lineEdit_FA"));
        lineEdit_FA->setMinimumSize(QSize(0, 25));

        verticalLayout_6->addWidget(lineEdit_FA);


        horizontalLayout_2->addLayout(verticalLayout_6);


        formLayout->setLayout(0, QFormLayout::LabelRole, horizontalLayout_2);

        stackedWidget->addWidget(page_AC);
        page_ID = new QWidget();
        page_ID->setObjectName(QString::fromUtf8("page_ID"));
        gridLayout = new QGridLayout(page_ID);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tw_peid_1 = new QTableWidget(page_ID);
        if (tw_peid_1->columnCount() < 7)
            tw_peid_1->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem34 = new QTableWidgetItem();
        tw_peid_1->setHorizontalHeaderItem(0, __qtablewidgetitem34);
        QTableWidgetItem *__qtablewidgetitem35 = new QTableWidgetItem();
        tw_peid_1->setHorizontalHeaderItem(1, __qtablewidgetitem35);
        QTableWidgetItem *__qtablewidgetitem36 = new QTableWidgetItem();
        tw_peid_1->setHorizontalHeaderItem(2, __qtablewidgetitem36);
        QTableWidgetItem *__qtablewidgetitem37 = new QTableWidgetItem();
        tw_peid_1->setHorizontalHeaderItem(3, __qtablewidgetitem37);
        QTableWidgetItem *__qtablewidgetitem38 = new QTableWidgetItem();
        tw_peid_1->setHorizontalHeaderItem(4, __qtablewidgetitem38);
        QTableWidgetItem *__qtablewidgetitem39 = new QTableWidgetItem();
        tw_peid_1->setHorizontalHeaderItem(5, __qtablewidgetitem39);
        QTableWidgetItem *__qtablewidgetitem40 = new QTableWidgetItem();
        tw_peid_1->setHorizontalHeaderItem(6, __qtablewidgetitem40);
        tw_peid_1->setObjectName(QString::fromUtf8("tw_peid_1"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tw_peid_1->sizePolicy().hasHeightForWidth());
        tw_peid_1->setSizePolicy(sizePolicy);

        gridLayout->addWidget(tw_peid_1, 0, 0, 1, 1);

        tw_peid_2 = new QTableWidget(page_ID);
        if (tw_peid_2->columnCount() < 4)
            tw_peid_2->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem41 = new QTableWidgetItem();
        tw_peid_2->setHorizontalHeaderItem(0, __qtablewidgetitem41);
        QTableWidgetItem *__qtablewidgetitem42 = new QTableWidgetItem();
        tw_peid_2->setHorizontalHeaderItem(1, __qtablewidgetitem42);
        QTableWidgetItem *__qtablewidgetitem43 = new QTableWidgetItem();
        tw_peid_2->setHorizontalHeaderItem(2, __qtablewidgetitem43);
        QTableWidgetItem *__qtablewidgetitem44 = new QTableWidgetItem();
        tw_peid_2->setHorizontalHeaderItem(3, __qtablewidgetitem44);
        tw_peid_2->setObjectName(QString::fromUtf8("tw_peid_2"));

        gridLayout->addWidget(tw_peid_2, 1, 0, 1, 1);

        stackedWidget->addWidget(page_ID);
        page_tls_dir = new QWidget();
        page_tls_dir->setObjectName(QString::fromUtf8("page_tls_dir"));
        gridLayout_5 = new QGridLayout(page_tls_dir);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        tw_petls = new QTableWidget(page_tls_dir);
        if (tw_petls->columnCount() < 4)
            tw_petls->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem45 = new QTableWidgetItem();
        tw_petls->setHorizontalHeaderItem(0, __qtablewidgetitem45);
        QTableWidgetItem *__qtablewidgetitem46 = new QTableWidgetItem();
        tw_petls->setHorizontalHeaderItem(1, __qtablewidgetitem46);
        QTableWidgetItem *__qtablewidgetitem47 = new QTableWidgetItem();
        tw_petls->setHorizontalHeaderItem(2, __qtablewidgetitem47);
        QTableWidgetItem *__qtablewidgetitem48 = new QTableWidgetItem();
        tw_petls->setHorizontalHeaderItem(3, __qtablewidgetitem48);
        tw_petls->setObjectName(QString::fromUtf8("tw_petls"));

        gridLayout_5->addWidget(tw_petls, 0, 0, 1, 1);

        stackedWidget->addWidget(page_tls_dir);
        page_RD = new QWidget();
        page_RD->setObjectName(QString::fromUtf8("page_RD"));
        gridLayout_4 = new QGridLayout(page_RD);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tw_perd_1 = new QTableWidget(page_RD);
        if (tw_perd_1->columnCount() < 3)
            tw_perd_1->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem49 = new QTableWidgetItem();
        tw_perd_1->setHorizontalHeaderItem(0, __qtablewidgetitem49);
        QTableWidgetItem *__qtablewidgetitem50 = new QTableWidgetItem();
        tw_perd_1->setHorizontalHeaderItem(1, __qtablewidgetitem50);
        QTableWidgetItem *__qtablewidgetitem51 = new QTableWidgetItem();
        tw_perd_1->setHorizontalHeaderItem(2, __qtablewidgetitem51);
        tw_perd_1->setObjectName(QString::fromUtf8("tw_perd_1"));

        gridLayout_3->addWidget(tw_perd_1, 0, 0, 1, 1);

        tw_perd_2 = new QTableWidget(page_RD);
        if (tw_perd_2->columnCount() < 3)
            tw_perd_2->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem52 = new QTableWidgetItem();
        tw_perd_2->setHorizontalHeaderItem(0, __qtablewidgetitem52);
        QTableWidgetItem *__qtablewidgetitem53 = new QTableWidgetItem();
        tw_perd_2->setHorizontalHeaderItem(1, __qtablewidgetitem53);
        QTableWidgetItem *__qtablewidgetitem54 = new QTableWidgetItem();
        tw_perd_2->setHorizontalHeaderItem(2, __qtablewidgetitem54);
        tw_perd_2->setObjectName(QString::fromUtf8("tw_perd_2"));

        gridLayout_3->addWidget(tw_perd_2, 1, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);

        stackedWidget->addWidget(page_RD);

        horizontalLayout->addWidget(stackedWidget);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 4);

        verticalLayout_2->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1114, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Member", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Offset", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Size", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Value", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tw_pesh_2->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tw_pesh_2->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Virtual Size", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tw_pesh_2->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Virtual Address", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tw_pesh_2->horizontalHeaderItem(3);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "Raw Size", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tw_pesh_2->horizontalHeaderItem(4);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "Raw Address", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tw_pesh_2->horizontalHeaderItem(5);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "Reloc Address", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tw_pesh_2->horizontalHeaderItem(6);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "Linenumbers", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tw_pesh_2->horizontalHeaderItem(7);
        ___qtablewidgetitem11->setText(QApplication::translate("MainWindow", "Relocations Number", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tw_pesh_2->horizontalHeaderItem(8);
        ___qtablewidgetitem12->setText(QApplication::translate("MainWindow", "Linenumbers Number", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tw_pesh_2->horizontalHeaderItem(9);
        ___qtablewidgetitem13->setText(QApplication::translate("MainWindow", "Characteristics", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tw_pesh_1->horizontalHeaderItem(0);
        ___qtablewidgetitem14->setText(QApplication::translate("MainWindow", "Byte[8]", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tw_pesh_1->horizontalHeaderItem(1);
        ___qtablewidgetitem15->setText(QApplication::translate("MainWindow", "Dword", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tw_pesh_1->horizontalHeaderItem(2);
        ___qtablewidgetitem16->setText(QApplication::translate("MainWindow", "Dword", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tw_pesh_1->horizontalHeaderItem(3);
        ___qtablewidgetitem17->setText(QApplication::translate("MainWindow", "Dword", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tw_pesh_1->horizontalHeaderItem(4);
        ___qtablewidgetitem18->setText(QApplication::translate("MainWindow", "Dword", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tw_pesh_1->horizontalHeaderItem(5);
        ___qtablewidgetitem19->setText(QApplication::translate("MainWindow", "Dword", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tw_pesh_1->horizontalHeaderItem(6);
        ___qtablewidgetitem20->setText(QApplication::translate("MainWindow", "Dword", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tw_pesh_1->horizontalHeaderItem(7);
        ___qtablewidgetitem21->setText(QApplication::translate("MainWindow", "Word", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tw_pesh_1->horizontalHeaderItem(8);
        ___qtablewidgetitem22->setText(QApplication::translate("MainWindow", "Word", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = tw_pesh_1->horizontalHeaderItem(9);
        ___qtablewidgetitem23->setText(QApplication::translate("MainWindow", "Dword", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = tw_export_dir_1->horizontalHeaderItem(0);
        ___qtablewidgetitem24->setText(QApplication::translate("MainWindow", "Member", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = tw_export_dir_1->horizontalHeaderItem(1);
        ___qtablewidgetitem25->setText(QApplication::translate("MainWindow", "Offset", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = tw_export_dir_1->horizontalHeaderItem(2);
        ___qtablewidgetitem26->setText(QApplication::translate("MainWindow", "Size", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = tw_export_dir_1->horizontalHeaderItem(3);
        ___qtablewidgetitem27->setText(QApplication::translate("MainWindow", "Value", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = tw_export_dir_2->horizontalHeaderItem(0);
        ___qtablewidgetitem28->setText(QApplication::translate("MainWindow", "Ordinal", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = tw_export_dir_2->horizontalHeaderItem(1);
        ___qtablewidgetitem29->setText(QApplication::translate("MainWindow", "Function RVA", nullptr));
        QTableWidgetItem *___qtablewidgetitem30 = tw_export_dir_2->horizontalHeaderItem(2);
        ___qtablewidgetitem30->setText(QApplication::translate("MainWindow", "Name Ordinal", nullptr));
        QTableWidgetItem *___qtablewidgetitem31 = tw_export_dir_2->horizontalHeaderItem(3);
        ___qtablewidgetitem31->setText(QApplication::translate("MainWindow", "Name RVA", nullptr));
        QTableWidgetItem *___qtablewidgetitem32 = tw_export_dir_2->horizontalHeaderItem(4);
        ___qtablewidgetitem32->setText(QApplication::translate("MainWindow", "Name", nullptr));
        label->setText(QApplication::translate("MainWindow", "VA", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "RVA", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "File Offset", nullptr));
        QTableWidgetItem *___qtablewidgetitem33 = tw_peid_1->horizontalHeaderItem(0);
        ___qtablewidgetitem33->setText(QApplication::translate("MainWindow", "Module Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem34 = tw_peid_1->horizontalHeaderItem(1);
        ___qtablewidgetitem34->setText(QApplication::translate("MainWindow", "Imports", nullptr));
        QTableWidgetItem *___qtablewidgetitem35 = tw_peid_1->horizontalHeaderItem(2);
        ___qtablewidgetitem35->setText(QApplication::translate("MainWindow", "OFTs", nullptr));
        QTableWidgetItem *___qtablewidgetitem36 = tw_peid_1->horizontalHeaderItem(3);
        ___qtablewidgetitem36->setText(QApplication::translate("MainWindow", "TimeDateStamp", nullptr));
        QTableWidgetItem *___qtablewidgetitem37 = tw_peid_1->horizontalHeaderItem(4);
        ___qtablewidgetitem37->setText(QApplication::translate("MainWindow", "ForwarderChain", nullptr));
        QTableWidgetItem *___qtablewidgetitem38 = tw_peid_1->horizontalHeaderItem(5);
        ___qtablewidgetitem38->setText(QApplication::translate("MainWindow", "Name RVA", nullptr));
        QTableWidgetItem *___qtablewidgetitem39 = tw_peid_1->horizontalHeaderItem(6);
        ___qtablewidgetitem39->setText(QApplication::translate("MainWindow", "FTs(IAT)", nullptr));
        QTableWidgetItem *___qtablewidgetitem40 = tw_peid_2->horizontalHeaderItem(0);
        ___qtablewidgetitem40->setText(QApplication::translate("MainWindow", "OFTs", nullptr));
        QTableWidgetItem *___qtablewidgetitem41 = tw_peid_2->horizontalHeaderItem(1);
        ___qtablewidgetitem41->setText(QApplication::translate("MainWindow", "FTs(IAT)", nullptr));
        QTableWidgetItem *___qtablewidgetitem42 = tw_peid_2->horizontalHeaderItem(2);
        ___qtablewidgetitem42->setText(QApplication::translate("MainWindow", "Hint", nullptr));
        QTableWidgetItem *___qtablewidgetitem43 = tw_peid_2->horizontalHeaderItem(3);
        ___qtablewidgetitem43->setText(QApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem44 = tw_petls->horizontalHeaderItem(0);
        ___qtablewidgetitem44->setText(QApplication::translate("MainWindow", "Member", nullptr));
        QTableWidgetItem *___qtablewidgetitem45 = tw_petls->horizontalHeaderItem(1);
        ___qtablewidgetitem45->setText(QApplication::translate("MainWindow", "Offset", nullptr));
        QTableWidgetItem *___qtablewidgetitem46 = tw_petls->horizontalHeaderItem(2);
        ___qtablewidgetitem46->setText(QApplication::translate("MainWindow", "Size", nullptr));
        QTableWidgetItem *___qtablewidgetitem47 = tw_petls->horizontalHeaderItem(3);
        ___qtablewidgetitem47->setText(QApplication::translate("MainWindow", "Value", nullptr));
        QTableWidgetItem *___qtablewidgetitem48 = tw_perd_1->horizontalHeaderItem(0);
        ___qtablewidgetitem48->setText(QApplication::translate("MainWindow", "RVA", nullptr));
        QTableWidgetItem *___qtablewidgetitem49 = tw_perd_1->horizontalHeaderItem(1);
        ___qtablewidgetitem49->setText(QApplication::translate("MainWindow", "Size Of Block", nullptr));
        QTableWidgetItem *___qtablewidgetitem50 = tw_perd_1->horizontalHeaderItem(2);
        ___qtablewidgetitem50->setText(QApplication::translate("MainWindow", "Items", nullptr));
        QTableWidgetItem *___qtablewidgetitem51 = tw_perd_2->horizontalHeaderItem(0);
        ___qtablewidgetitem51->setText(QApplication::translate("MainWindow", "Item", nullptr));
        QTableWidgetItem *___qtablewidgetitem52 = tw_perd_2->horizontalHeaderItem(1);
        ___qtablewidgetitem52->setText(QApplication::translate("MainWindow", "RVA", nullptr));
        QTableWidgetItem *___qtablewidgetitem53 = tw_perd_2->horizontalHeaderItem(2);
        ___qtablewidgetitem53->setText(QApplication::translate("MainWindow", "Type", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
