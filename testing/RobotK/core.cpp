#include "core.h"

Core::Core(JoystickControl* joyP, MainWindow* winP)
{
  lastSend = QTime::currentTime();
  updateTime = 50; //ms
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
  //  emit sig_joystickPositionChanged(arg1,arg2,arg3);
}

void Core::send(QString qstringCommand)
{
  if(socket.isWritable())
  {
    socket.write(qstringCommand.toStdString().c_str());
    lastSend = QTime::currentTime();
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


