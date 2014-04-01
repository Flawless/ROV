#include "core.h"

#include "math.h"

Core::Core(JoystickControl* joyP, MainWindow* winP)
{
  pJoystickControl = joyP;
  pWindow = winP;
  //JOYSTICK POSITION
  QObject::connect(pJoystickControl, SIGNAL(axisEvent(int, int, int)), pWindow, SLOT(slot_joystickPositionChanged(int, int, int)));
  QObject::connect(pJoystickControl, SIGNAL(axisEvent(int, int, int)), this, SLOT(slot_joystickPositionChanged(int, int, int)));
  joyList=pJoystickControl->GetJoystickNames();
  pWindow->slot_newJoyList(joyList);
  pJoystickControl->Initialize(joyList[0], updateTime);
  pWindow->setUiSlidersMaximum(axisMax);
  if (debug)
  {
    QObject::connect(pJoystickControl, SIGNAL(axisEvent(int, int, int)), this, SLOT(slot_debug_PrintAxisPosition(int, int, int)));
  }
}
void Core::slot_debug_PrintAxisPosition(int arg1, int arg2, int arg3)
{
  printf("arg1: %i ", arg1);
  printf("; arg2: %i ", arg2);
  printf("; arg3: %i \n", arg3);
}
void Core::slot_connect(QString ip, int port)
{
  socket.connectToHost(ip, port);
}
void Core::slot_disconnect()
{
  socket.disconnect();
}
void Core::slot_positionControlTypeChanged(int controlType) //0-butonns 1-manual 2-joystick
{
  if (controlType!=2)
  {
    QObject::disconnect(pJoystickControl, SIGNAL(axisEvent(int, int, int)), this, SLOT(slot_joystickPositionChanged(int, int, int)));
    QObject::disconnect(pJoystickControl, SIGNAL(axisEvent(int, int, int)), pWindow, SLOT(slot_joystickPositionChanged(int, int, int)));
  }
  else if (controlType==2)
  {
    QObject::connect(pJoystickControl, SIGNAL(axisEvent(int, int, int)), this, SLOT(slot_joystickPositionChanged(int, int, int)));
    QObject::connect(pJoystickControl, SIGNAL(axisEvent(int, int, int)), pWindow, SLOT(slot_joystickPositionChanged(int, int, int)));
  }
}
void Core::slot_joystickSelected(QString joystickName)
{
  pJoystickControl->currentJoystickName = joystickName;
  pJoystickControl->Initialize(joystickName, 500);
}
void Core::slot_joystickPositionChanged(int arg1, int arg2, int arg3)
{
  QString command=makeSpeedCommand(arg1, arg2);
  send(command);
}
QString Core::makeSpeedCommand(int x, int y)
{
  double angle;
  int length = sqrt(y*y + x*x);
  angle=atan2(x, y) - M_PI/2;
  angle *= 180/M_PI;
  if (angle<0)
    angle+=360;
  return "#mh."+QString::number(round(angle))+"."+QString::number(length)+"!";
}
void Core::send(QString qstringCommand)
{
  if(socket.isWritable())
  {
    socket.write(qstringCommand.toStdString().c_str());
    printf("command sent");
    printf(qstringCommand.toStdString().c_str());
    printf("\n");
  }
}

// void Core::sendCommand(ArgList command, bool timeCheck)
// {
//   currenttime =  QTime::currentTime();
//   if(command.name=="mh" && lastSend<currentTime.addMSecs(-500) && timeCheck)
//   {
//     qstringCommand = command.name;
//     for (int i = 0; i < command.args.size(); ++i)
//       qstringCommand += "." + command.args[i];
//       send(qstringCommand);
//   }
//   lastCommand = command;
// }


