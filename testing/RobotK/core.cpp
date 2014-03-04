#include "core.h"
#include "joystickcontrol.h"

Core::Core()
{
  lastSend = QTime::currentTime();

  updateTime = 50; //ms
}

void Core::socket_connect(QString ip, int port)
{
  socket.connectToHost(ip, port);
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


