#include "mainwindow.h"
#include "core.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MainWindow* pointer_w = &w;
    Core core(pointer_w);
    w.show();

    return a.exec();
}
