#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T14:01:53
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wu-zi-qi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gamemodel.cpp \
    start.cpp

HEADERS  += mainwindow.h \
    gamemodel.h \
    start.h

FORMS    += mainwindow.ui \
    start.ui

RESOURCES += \
    res.qrc

DISTFILES +=
