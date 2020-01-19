#-------------------------------------------------
#
# Project created by QtCreator 2020-01-16T09:29:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = autobrightness-daemon
TEMPLATE = app


inst1.file += ../data/autobrightness-daemon.service
inst1.path = /lib/systemd/system/

target.source += $$TARGET
target.path =/usr/bin/

INSTALLS += inst1 \
            target \


SOURCES += main.cpp\
        daemon.cpp \
    simpleobject.cpp

HEADERS  += daemon.h \
    simpleobject.h

FORMS    +=
