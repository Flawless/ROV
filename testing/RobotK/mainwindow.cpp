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
        ui->lineEdit_command->clear();
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


void MainWindow::on_lineEdit_command_returnPressed()
{
    on_pushButton_sendCommand_clicked();
}

void MainWindow::on_pushButton_up_clicked()
{
    sendCommand("#mh.0." + QString::number((ui->spinBox_power->value())) + "!");
}

void MainWindow::on_pushButton_up_right_clicked()
{
    sendCommand("#mh.45." + QString::number((ui->spinBox_power->value())) + "!");
}

void MainWindow::on_pushButton_right_clicked()
{
    sendCommand("#mh.90." + QString::number((ui->spinBox_power->value())) + "!");
}

void MainWindow::on_pushButton_down_right_clicked()
{
    sendCommand("#mh.135." + QString::number((ui->spinBox_power->value())) + "!");
}

void MainWindow::on_pushButton_down_clicked()
{
    sendCommand("#mh.180." + QString::number((ui->spinBox_power->value())) + "!");
}

void MainWindow::on_pushButton_down_left_clicked()
{
    sendCommand("#mh.225." + QString::number((ui->spinBox_power->value())) + "!");
}

void MainWindow::on_pushButton_left_clicked()
{
    sendCommand("#mh.270." + QString::number((ui->spinBox_power->value())) + "!");
}

void MainWindow::on_pushButton_up_left_clicked()
{
    sendCommand("#mh.315." + QString::number((ui->spinBox_power->value())) + "!");
}
