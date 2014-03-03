#include "core.h"
#include "joystickcontrol.h"

Core::Core(w)
{
  pWindow=w;
  lastSend = QTime::currentTime();

  updateTime = 50; //ms

  connect(&socket, SIGNAL(connected()),    this, SLOT(socket_connected()));
  connect(&socket, SIGNAL(disconnected()), this, SLOT(socket_disconnected()));
}

connect()
{
  QString ip = MainWindow::host_ip();
  int port   = MainWindow::host_port();
  socket.connectToHost(ip, port);
}

joystick_axisChanged(int arg1, int arg2, int arg3)
{
}

send(QString qstringCommand)
{
  if(socket.isWritable())
  {
    socket.write(qstringCommand.toStdString().c_str());
    lastSend = QTime::currentTime();
    
    ui->textEdit_commandsLog->append(qstringCommand.toStdString().c_str());
    ui->lineEdit_command->clear();
  }
}

void MainWindow::sendCommand(ArgList command, bool timeCheck)
{
  currentTime =  QTime::currentTime();
  if(command.name=="mh" && lastSend<currentTime.addMSecs(-500) && timeCheck)
  {
    qstringCommand = command.name;
    for (int i = 0; i < command.args.size(); ++i)
      qstringCommand += "." + command.args[i];
      send(qstringCommand);
  }
  lastCommand = command;
}

//GUI requests


request_JoustickNames()
{
  return JoystickControl::GetJoystickNames();
}

