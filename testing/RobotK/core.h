#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QTimer>
#include <QString>

#include "joystickcontrol.h"
#include "mainwindow.h"

class Core : public QObject
{
  Q_OBJECT
public:
  Core(JoystickControl* joyP, MainWindow* winP);
  QStringList question_JoystickNames();
  void send(QString qstringCommand);
private:
  const int axisMax = 32768;
  const bool debug = true;
  QStringList joyList;
  QString makeSpeedCommand(int x, int y);
  QTcpSocket socket;
  QTimer     timer;
  const int  updateTime = 50;
  MainWindow* pWindow;
  JoystickControl* pJoystickControl;
public slots:
  /* void socked_connected(); */
  /* void socked_disconnected();  */
  void slot_connect(QString ip, int port);
  void slot_disconnect();
  void slot_positionControlTypeChanged(int cType);
  void slot_joystickSelected(QString joystickName);
  void slot_joystickPositionChanged(int arg1, int arg2, int arg3);
  //debug
  void slot_debug_PrintAxisPosition(int arg1, int arg2, int arg3);
signals:
  void sig_newJoyList(QStringList joyList);
};

#endif // CORE_H
