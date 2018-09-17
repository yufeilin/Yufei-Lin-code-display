#-------------------------------------------------
#
# Project created by QtCreator 2016-11-20T17:40:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 11201
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    spydialog.cpp \
    spywidget.cpp \
    doswidget.cpp \
#    tiaozhangbao.cpp \
    threadtiaozhang.cpp \
    hh.cpp \
    threadhh.cpp \
    hostfindwidget.cpp \
    hhpage2.cpp \
    taskhandler.cpp \
    get_gateway.cpp \
    arpwidget.cpp \
    arp.cpp \
    arpthread.cpp

HEADERS  += widget.h \
    spydialog.h \
    spywidget.h \
    doswidget.h \
    threadtiaozhang.h \
    hhpacket.h \
    threadhh.h \
    hostfindwidget.h \
    taskhandler.h \
    arpwidget.h \
    arpthread.h

FORMS    += widget.ui \
    spydialog.ui \
    spywidget.ui \
    doswidget.ui \
    hostfindwidget.ui \
    arpwidget.ui

RESOURCES += \
    icon.qrc


LIBS += -L/usr/local/lib -lpcap -lnet
