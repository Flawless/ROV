#include "mainwindow.h"
#include "core.h"
#include <QApplication>
#include <QtCore/QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Core core;
    MainWindow w(&core);
    //CONNECTION
    QObject::connect(&w, SIGNAL(sig_connect(QString, int)), &core, SLOT(slot_connect(QString, int)));
    QObject::connect(&w, SIGNAL(sig_disconnect(QString, int)), &core, SLOT(slot_disconnect(QString, int)));
    
    //CONTROL TYPE CHANGE
    QObject::connect(&w, SIGNAL(sig_positionControlTypeChanged(int)), &core, SLOT(slot_positionControlTypeChanged(int)));
    
    //JOYSTICK CHANGE
    QObject::connect(&w, SIGNAL(sig_joystickChanged(QString)), &core, SLOT(slot_joystickChaged(QString)));








    w.show();

 
   
    return a.exec();
}
