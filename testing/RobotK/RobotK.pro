#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T23:48:44
#
#-------------------------------------------------

QT       += core gui\
	    network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobotK
TEMPLATE = app

LIBS += -lSDL


SOURCES += main.cpp mainwindow.cpp joystickcontrol.cpp other.cpp core.cpp

HEADERS  += mainwindow.h joystickcontrol.h other.h core.h

FORMS    += mainwindow.ui
