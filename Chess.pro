#-------------------------------------------------
#
# Project created by QtCreator 2019-03-26T21:05:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chess
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

CONFIG += c++17

SOURCES += \
    source/widgets/main.cpp \
    source/widgets/mainwindow.cpp \
	source/widgets/pieceitem.cpp \
    source/game/piece.cpp \
    source/game/game.cpp \
    source/game/position.cpp \
    source/widgets/graphicsview.cpp \
    source/game/piecemovements.cpp \
    source/game/iobserver.cpp \
    source/game/commands.cpp \
    source/game/history.cpp \
    source/widgets/historymodel.cpp \
    source/widgets/historydelegate.cpp


HEADERS += \
    source/widgets/mainwindow.h \
	source/widgets/pieceitem.h \
    source/game/piece.h \
    source/game/game.h \
    source/game/iobserver.h \
    source/game/gamecommon.h \
    source/game/position.h \
    source/game/defs.h \
    source/widgets/graphicsview.h \
    source/game/ipiecemovement.h \
    source/game/piecemovements.h \
    source/game/commands.h \
    source/game/history.h \
    source/widgets/historymodel.h \
    source/widgets/historydelegate.h \
    source/widgets/widgettools.h

FORMS += \
    source/widgets/mainwindow.ui

INCLUDEPATH += source

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
