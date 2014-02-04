QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobotControlGUI
TEMPLATE = app

INCLUDEPATH += ..\

CONFIG += serialport

DEFINES += "_USE_MATH_DEFINES"

LIBS += -lSDL
#LIBS += libSerialPort

SOURCES += *.cpp \
    ../robot-control/robotcontrol.cpp \
    ../joystick/joystickcontrol.cpp

HEADERS += *.h \
    ../joystick/joystickcontrol.h \
    ../robot-control/robotcontrol.h

FORMS += widget.ui
