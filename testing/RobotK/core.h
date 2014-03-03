#ifndef CORE_H
#define CORE_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QTime>
#include <QString>

class Core
{
public:
    Core(MainWindow*w);
    QStringList question_JoystickNames();
private:
    MainWindow *pWindow;

    void joystick_axisChanged(int arg1, int arg2, int arg3);
    void socked_connected();
    void socked_disconnected();
    void on_pushButton_connect_clicked();
    void sendCommand(ArgList command, bool timeCheck);
    void send(QString qstringCommand);
signals:

};

#endif // CORE_H
