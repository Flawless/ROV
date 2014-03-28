#include "core.h"

#include "math.h"

Core::Core(JoystickControl* joyP, MainWindow* winP)
{
  pJoystickControl = joyP;
  pWindow = winP;
  //JOYSTICK POSITION
  QObject::connect(pJoystickControl, SIGNAL(axisEvent(int, int, int)), pWindow, SLOT(slot_joystickPositionChanged(int, int, int)));
}
void Core::slot_connect(QString ip, int port)
{
  socket.connectToHost(ip, port);
}
void Core::slot_disconnect()
{
  socket.disconnect();
}
void Core::slot_positionControlTypeChanged(int controlType)
{
  positionControlType = controlType;
}
void Core::slot_joystickSelected(QString joystickName)
{
  pJoystickControl->currentJoystickName = joystickName;
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
  if(x>0 && y>=0)
    angle = atan(y/x);
  else if(x>0 && y<0)
    angle = atan(y/x) + 2*M_PI;
  else if(x<0)
    angle = atan(y/x) + M_PI;
  else if(x==0 && y>0)
    angle = M_PI_2;
  else if(x==0 && y>0)
    angle = 3*M_PI_2;
  else if(x==0 && y==0)
    angle = 0;
  angle *= 180/M_PI;
  return "#mh."+QString::number(round(angle))+"."+QString::number(length)+"!";
}
void Core::send(QString qstringCommand)
{
  if(socket.isWritable())
  {
    socket.write(qstringCommand.toStdString().c_str());
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


