#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H

#include <QTimer>
#include <QVector>
#include <QStringList>
#include <QtNetwork/QTcpSocket>

#include "auto_ptr.h"
class Command
{
public:
  QString name;
  QVector<char> args;
};

class RobotControl : public QObject
{
  Q_OBJECT

public:
  struct moveSpeed
  {
    int angle,
    speed;
  };
  explicit RobotControl(QObject *parent = 0);

  ~RobotControl();

  int Initialize(QString address, int port, int _tickTime);

  void SetVerticalSpeed(int value);

  void SetMoveSpeed(int speed, int angle);

  void SetRotateSpeed(int value);

  void SetPitchSpeed(int value);

  void SetHalt(bool state);

  void SetPitching(bool state);

  void StartEngines();

  void StopEngines();

  void StopWriting();

  void StartWriting();

  int GetSpeed();

  bool GetReverse();

  bool GetHalt();

  double GetDepth();

  double GetPitch();

  void SetTargetDepth(double tgDepth);

  void SetTargetPitch(double tgPitch);

  void SetManualControl(bool state);

  void SetPitchReg(bool state);

  void SetDepthReg(bool state);

  bool EnginesStarted();

  void OpenManip(int commands);

  void CloseManip(int commands);

  //void WriteSpeed(int speed, int engine);

  //void WriteReverse(int reverse, int engine);

  void WriteCommand(char *commandToWrite);

  char MakeCommand(Command cV);

protected:
  QTcpSocket socket;
  QTimer timer;
  int tickTime,
  ticksForReverse;
  int vertSpeed,
  rotateSpeed,
  pitchSpeed;
  bool initialized, manualControl;


  int     newSpeed,
  actualSpeed;
  double  angle,
  coeff;

  enum State
  {
    Stop,
    Move,
    Yaw,
    Pitch,
    Roll
  } currentState;

  //    virtual void CalcEnginesData();

private slots:
  void TimerTick();

  void SocketDisconnected();

signals:
  void Disconnected();
};

#endif // ROBOTCONTROL_H
