#include "mainwindow.h"
#include "core.h"
#include <QApplication>
#include <QtCore/QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Core core;
    QObject::connect(&w, SIGNAL(socket_connect(QString, int)), &core, SLOT(socket_connect(QString, int)));
    w.show();
    
    return a.exec();
}
