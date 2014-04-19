#-------------------------------------------------
#
# Project created by QtCreator 2014-04-11T21:37:29
#
#-------------------------------------------------

QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mdeditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mdrenderer.cpp \
    hoedown/src/autolink.c \
    hoedown/src/buffer.c \
    hoedown/src/escape.c \
    hoedown/src/html.c \
    hoedown/src/html_blocks.c \
    hoedown/src/html_smartypants.c \
    hoedown/src/stack.c \
    hoedown/src/markdown.c

HEADERS  += mainwindow.h \
    mdrenderer.h \
    hoedown/src/autolink.h \
    hoedown/src/buffer.h \
    hoedown/src/escape.h \
    hoedown/src/html.h \
    hoedown/src/markdown.h \
    hoedown/src/stack.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
