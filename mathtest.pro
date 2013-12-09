#-------------------------------------------------
#
# Project created by QtCreator 2011-11-20T12:55:05
#
#-------------------------------------------------

QT       += core gui

TARGET = mathtest
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += mathtest.cpp \
    defaultdlg.cpp \
    mt-main.cpp

HEADERS  += mathtest.h\
    defaultdlg.h \
    mtnamespace.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../qpgui/release/ -lqpgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../qpgui/debug/ -lqpgui
else:symbian: LIBS += -lqpgui
else:unix: LIBS += -L$$PWD/../qpgui/ -lqpgui

INCLUDEPATH += $$PWD/../qpgui
DEPENDPATH += $$PWD/../qpgui

debug {
    DESTDIR = $$PWD/debug
}

release {
    DESTDIR = $$PWD/release
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../mathpack/release/ -lmathpack
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../mathpack/debug/ -lmathpack
else:symbian: LIBS += -lmathpack
else:unix: LIBS += -L$$PWD/../mathpack/ -lmathpack

INCLUDEPATH += $$PWD/../mathpack
DEPENDPATH += $$PWD/../mathpack







