#include "core.h"
//#include "joystickcontrol.h"

Core::Core()
{
  lastSend = QTime::currentTime();
  updateTime = 50; //ms
  JoystickControl joystickControl;
  pJoystickControl = &joystickControl;
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
void Core::slot_joystickChaged(QString joystickName)
{
  pJoystickControl->currentJoystickName = joystickName;
}

// Core::joystick_axisChanged(int arg1, int arg2, int arg3)
// {
// }

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


