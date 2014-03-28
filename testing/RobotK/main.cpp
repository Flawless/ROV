#include "mainwindow.h"
#include "core.h"
#include "joystickcontrol.h"
#include <QApplication>
#include <QtCore/QObject>

int main(int argc, char *argv[])
{
  JoystickControl joystickcontrol;
  QApplication a(argc, argv);
  MainWindow w;
  Core core(&joystickcontrol, &w);
  //CONNECTION
  QObject::connect(&w, SIGNAL(sig_connect(QString, int)), &core, SLOT(slot_connect(QString, int)));
  QObject::connect(&w, SIGNAL(sig_disconnect(QString, int)), &core, SLOT(slot_disconnect(QString, int)));
    
  //CONTROL TYPE CHANGE
  QObject::connect(&w, SIGNAL(sig_positionControlTypeChanged(int)), &core, SLOT(slot_positionControlTypeChanged(int)));
    
  //JOYSTICK NAME CHANGE
  QObject::connect(&w, SIGNAL(sig_joystickChanged(QString)), &core, SLOT(slot_joystickSelected(QString)));

  w.show();

 
   
  return a.exec();
}
