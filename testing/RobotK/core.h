#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QTime>
#include <QString>

class Core : public QObject
{
  Q_OBJECT
public:
    Core();
    QStringList question_JoystickNames();
    void send(QString qstringCommand);
private:
    QTcpSocket socket;
    QTime lastSend;
    int updateTime;
public slots:
    /* void joystick_axisChanged(int arg1, int arg2, int arg3); */
    /* void socked_connected(); */
    /* void socked_disconnected();  */
    void socket_connect(QString ip, int port);
    /* void sendCommand(ArgList command, bool timeCheck); */
signals:
    /* void commandSent(QString command); */
};

#endif // CORE_H
