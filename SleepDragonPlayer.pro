#-------------------------------------------------
#
# Project created by QtCreator 2019-01-16T08:58:48
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SleepDragonPlayer
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
        frmwidget.cpp \
    videothread.cpp \
    qsingleapplication.cpp \
    iconstyle.cpp \
    videowidget.cpp

HEADERS += \
        frmwidget.h \
    videothread.h \
    qsingleapplication.h \
    iconstyle.h \
    videowidget.h

FORMS += \
        frmwidget.ui

INCLUDEPATH += $$PWD/ffmpeg/include
DEPENDPATH += $$PWD/ffmpeg/include

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavcodec

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/ffmpeg/lib/avcodec.lib

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavformat

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/ffmpeg/lib/avformat.lib

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lswscale

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/ffmpeg/lib/swscale.lib

win32: LIBS += -L$$PWD/ffmpeg/lib/ -lavutil

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/ffmpeg/lib/avutil.lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
