#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QTime>
#include <QString>

#include "joystickcontrol.h"

class Core : public QObject
{
  Q_OBJECT
public:
    Core();
    QStringList question_JoystickNames();
    void send(QString qstringCommand);
    int positionControlType;
private:
    QTcpSocket socket;
    QTime lastSend;
    int updateTime;
    JoystickControl* pJoystickControl;
public slots:
    /* void joystick_axisChanged(int arg1, int arg2, int arg3); */
    /* void socked_connected(); */
    /* void socked_disconnected();  */
    void slot_connect(QString ip, int port);
    void slot_disconnect();
    void slot_positionControlTypeChanged(int cType);
    void slot_joystickChaged(QString joystickName);
    /* void sendCommand(ArgList command, bool timeCheck); */
signals:
    /* void commandSent(QString command); */
};

#endif // CORE_H
