
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mathtest
TEMPLATE = app

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../lib

win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib -lmathpack
else:unix: LIBS += -L$$PWD/../../lib -lmathpack

win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib -lqpgui
else:unix: LIBS += -L$$PWD/../../lib -lqpgui

SOURCES += mathtest.cpp \
    defaultdlg.cpp \
    mt-main.cpp

HEADERS  += mathtest.h\
    defaultdlg.h \
    mtnamespace.h

debug {
    DESTDIR = $$PWD/debug
}

release {
    DESTDIR = $$PWD/release
}
