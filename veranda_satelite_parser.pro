#-------------------------------------------------
#
# Project created by QtCreator 2014-09-29T14:37:18
#
#-------------------------------------------------

QT       += core

QT       -= gui
QT       += sql
QT       += network

TARGET    = veranda_tanker
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

OBJECTS_DIR = compile
MOC_DIR = compile

SOURCES += main.cpp \
    util/util_skyw.cpp \
    model/get_db.cpp \
    controller/worker.cpp \
    model/save_db.cpp \
    model/init_mysql.cpp \
    util/parsing_fuction.cpp \
    model/veranda_log.cpp

HEADERS += \
    util/util_skyw.h \
    model/get_db.h \
    controller/worker.h \
    model/save_db.h \
    model/init_mysql.h \
    util/parsing_fuction.h \
    util/utama.h \
    model/veranda_log.h
