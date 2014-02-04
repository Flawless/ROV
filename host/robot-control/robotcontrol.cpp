#include "robotcontrol.h"
#include <QStringList>
#include <QtNetwork/QTcpSocket>

//Radius radius;
//radius.length = sqrt(y*y + x*x);
//if(x>0 && y>=0)
//radius.angle = atan(y/x);
//else if(x>0 && y<0)
//radius.angle = atan(y/x) + 2*M_PI;
//else if(x<0)
//radius.angle = atan(y/x) + M_PI;
//else if(x==0 && y>0)
//radius.angle = M_PI_2;
//else if(x==0 && y>0)
//radius.angle = 3*M_PI_2;
//else if(x==0 && y==0)
//radius.angle = 0;
//radius.angle *= 180/M_PI;
//Command cV;
//cV.name = "mh";
//cV.args.append(char(radius.angle));
//cV.args.append(char(radius.length));
//char char_command = RobotControl::MakeCommand(cV);

RobotControl::RobotControl(QObject *parent) :
  QObject(parent),
  vertSpeed(0),
  //    moveSpeed(0),
  rotateSpeed(0),
  initialized(false),
  manualControl(false),
  currentState(Stop)
{
  connect(&socket, SIGNAL(disconnected()), this, SLOT(SocketDisconnected()));
}

RobotControl::~RobotControl()
{
  socket.close();
}

int RobotControl::Initialize(QString address, int port, int _tickTime)
{
  tickTime = _tickTime;
  ticksForReverse = qRound(2000.0/tickTime + 0.5); //ceil((2 seconds)/(one tick))

  QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(TimerTick()));

  timer.start(tickTime);

  socket.connectToHost(address, port);
  currentState = Move;
  initialized = true;
  return 1;
}

void RobotControl::SetVerticalSpeed(int value)
{
  vertSpeed = value;
}

void RobotControl::SetMoveSpeed(int speed, int angle)
{
  moveSpeed.speed = speed;
  moveSpeed.angle = angle;
}

void RobotControl::SetRotateSpeed(int value)
{
  rotateSpeed = value;
}

void RobotControl::SetPitchSpeed(int value)
{
  pitchSpeed = value;
}

void RobotControl::SetHalt(bool state)
{
  currentState = state ? Yaw : Move;
}

void RobotControl::SetPitching(bool state)
{
  currentState = state ? Pitch : Move;
}

void RobotControl::StartEngines()
{
  currentState = Move;
}

void RobotControl::StopEngines()
{
  currentState = Stop;
}

void RobotControl::OpenManip(int commands)
{
  QString toWrite = "";
  for (int i = 0; i < commands; i++)
    toWrite += "#m1!";
  if(socket.isWritable())
    socket.write(toWrite.toStdString().c_str());
}

void RobotControl::CloseManip(int commands)
{
  QString toWrite = "";
  for (int i = 0; i < commands; i++)
    toWrite += "#m0!";
  if(socket.isWritable())
    socket.write(toWrite.toStdString().c_str());
}

void RobotControl::WriteCommand(char* commandToWrite)
{
  if(socket.isWritable())
    socket.write(commandToWrite);
}

void RobotControl::TimerTick()
{
  //    WriteCommand();
}

void RobotControl::StartWriting()
{
  timer.start(tickTime);
}

void RobotControl::StopWriting()
{
  timer.stop();
}

bool RobotControl::EnginesStarted()
{
  if(!initialized)return false;
  return currentState != Stop ? true : false;
}

void RobotControl::SocketDisconnected()
{
  emit Disconnected();
}

void RobotControl::SetManualControl(bool state)
{
  manualControl = state;
}
