#ifndef ROBOTSOCKET_H
#define ROBOTSOCKET_H

#include <QTcpSocket>

class robotSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit robotSocket(QObject *parent = 0);

signals:

public slots:

};

#endif // ROBOTSOCKET_H
