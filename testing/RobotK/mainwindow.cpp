#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ip=ui->lineEdit_ip->text().convert(QHostAddress);
//    port=ui->lineEdit_portNumber->text().toInt();

    connect(&socket, SIGNAL(connected()),    this, SLOT(socket_connected()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(socked_disconnected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    socket.connectToHost(ui->lineEdit_ip->text(), ui->lineEdit_portNumber->text().toInt());
}


void MainWindow::socked_connected()
{
    ui->textEdit_commandsLog->append("connected");
//    ui->pushButton_connect->setEnabled(false);
//    ui->pushButton_disconnect->setEnabled(true);
}

void MainWindow::socked_disconnected()
{
    ui->textEdit_commandsLog->append("disconnected");
//    ui->pushButton_disconnect->setEnabled(false);
//    ui->pushButton_connect->setEnabled(true);
}


void MainWindow::sendCommand(QString command)
{
    if(socket.isWritable())
    {
        socket.write(command.toStdString().c_str());
        ui->textEdit_commandsLog->append(command.toStdString().c_str());
    }
}

//bool checkCommand(char command)
//{
//    return true;
//}


void MainWindow::on_pushButton_sendCommand_clicked()
{
//    if (checkCommand(ui->lineEdit_command->text()) then
            sendCommand(ui->lineEdit_command->text());
}
