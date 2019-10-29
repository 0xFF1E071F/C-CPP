#-------------------------------------------------
#
# Project created by QtCreator 2019-02-07T19:40:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TaskManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        MyWidget.cpp \
    TabDlg.cpp \
    ProcessTab.cpp \
    ProcessModuleDlg.cpp \
    ProcessThreadDlg.cpp \
    ServiceTab.cpp \
    TraversalWndTab.cpp \
    FileTab.cpp \
    RegEditTab.cpp \
    RegEditModDlg.cpp \
    SpyppTab.cpp \
    FullScreenLayer.cpp

HEADERS += \
        MyWidget.h \
    TabDlg.h \
    ProcessTab.h \
    ProcessModuleDlg.h \
    ProcessThreadDlg.h \
    ServiceTab.h \
    TraversalWndTab.h \
    FileTab.h \
    RegEditTab.h \
    RegEditModDlg.h \
    SpyppTab.h \
    FullScreenLayer.h

FORMS += \
        MyWidget.ui \
    ProcessModuleDlg.ui \
    ProcessThreadDlg.ui \
    ServiceTab.ui \
    TraversalWndTab.ui \
    FileTab.ui \
    RegEditTab.ui \
    RegEditModDlg.ui \
    SpyppTab.ui \
    FullScreenLayer.ui

#LIBS += -lAdvapi32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    myicon.qrc
