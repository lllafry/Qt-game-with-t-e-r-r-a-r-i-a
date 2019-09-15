#-------------------------------------------------
#
# Project created by QtCreator 2019-05-18T21:58:00
# сделал Петров Алексей, МП-22 (ИВТ-22)
# функция сохранения данных при закрытии не реализована,
# но её реализация легка, нужно просто создать массив с
# типом персонажа, его текущими координатами и список задач way
# v1.1 Дополнен функционал лифтов
# теперь они вмещают 10 людей
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = city
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp \
    sprite.cpp \
    lift.cpp \
    worker.cpp \
    citizen.cpp \
    npc.cpp \
    loader.cpp \
    settings_form.cpp \
    town.cpp

HEADERS  += \
    sprite.h \
    lift.h \
    worker.h \
    citizen.h \
    npc.h \
    loader.h \
    settings_form.h \
    town.h

OTHER_FILES +=

FORMS += \
    settings_form.ui

RESOURCES += \
    resource/res.qrc
