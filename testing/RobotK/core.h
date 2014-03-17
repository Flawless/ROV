#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QTime>
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
    int positionControlType;
private:
    struct ArgList
    {
      QString name;
      QList<QString> args;
    };
    struct Radius
    {
      double angle;
      int    length;
    };
    QTcpSocket socket;
    QTime      lastSend;
    int        updateTime;
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
    /* void sendCommand(ArgList command, bool timeCheck); */
signals:
    /* void commandSent(QString command); */
};

#endif // CORE_H
