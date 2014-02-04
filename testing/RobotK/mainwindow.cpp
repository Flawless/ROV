#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "joystickcontrol.h"
#include "other.h"
#include "math.h"

#include <QtWidgets/QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox_joystik->addItems(JoystickControl::GetJoystickNames());
//    ip=ui->lineEdit_ip->text().convert(QHostAddress);
//    port=ui->lineEdit_portNumber->text().toInt();

    connect(&socket, SIGNAL(connected()),    this, SLOT(socket_connected()));
    connect(&socket, SIGNAL(disconnected()), this, SLOT(socked_disconnected()));
    connect(&joystick, SIGNAL(axisEvent(int,int,int)), this, SLOT(joystick_axisChanged(int,int,int)));
    connect(&joystick, SIGNAL(buttonEvent(int,bool)), this, SLOT(joystick_buttonPressed(int,bool)));

    if(JoystickControl::GetJoystickNames().length() < 1)
      QMessageBox::warning(this, "Error", "No joysticks available!");
    else
      if(joystick.Initialize(ui->comboBox_joystik->currentText(), 50) < 0)
        QMessageBox::warning(this, "Error", "Couldn't open joystick!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::joystick_axisChanged(int arg1, int arg2, int arg3)
{
  if(joystickEnabled)
    {
      int x =     arg1 * 100 / 32768,
          y =  -  arg2 * 100 / 32768,
          z =     arg3 * 100 / 32768;
      ui->slider_y->setValue(y);
      ui->slider_z->setValue(z);
      ui->slider_x->setValue(x);
    }
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

void MainWindow::on_groupBox_manual_toggled(bool arg1)
{
  if(arg1) {
    ui->groupBox_buttons->setChecked(!arg1);
    ui->groupBox_joystick->setChecked(!arg1);
    joystickEnabled = !arg1;
  }
}

void MainWindow::on_groupBox_joystick_toggled(bool arg1)
{
  if(arg1) {
    ui->groupBox_buttons->setChecked(!arg1);
    ui->groupBox_manual->setChecked(!arg1);
    joystickEnabled = arg1;
  }
}

void MainWindow::on_groupBox_buttons_toggled(bool arg1)
{
  if(arg1) {
    ui->groupBox_manual->setChecked(!arg1);
    ui->groupBox_joystick->setChecked(!arg1);
    joystickEnabled = !arg1;
  }
}

void MainWindow::on_slider_x_valueChanged(int value)
{
  sendSpeedCommand(ui->slider_x->value(), ui->slider_y->value());

}

void MainWindow::on_slider_z_valueChanged(int value)
{

}

void MainWindow::on_slider_y_valueChanged(int value)
{
  sendSpeedCommand(ui->slider_x->value(), ui->slider_y->value());
}

void MainWindow::sendSpeedCommand(int x, int y)
{
  Radius radius;
  radius.length = sqrt(y*y + x*x);
  if(x>0 && y>=0)
  radius.angle = atan(y/x);
  else if(x>0 && y<0)
  radius.angle = atan(y/x) + 2*M_PI;
  else if(x<0)
  radius.angle = atan(y/x) + M_PI;
  else if(x==0 && y>0)
  radius.angle = M_PI_2;
  else if(x==0 && y>0)
  radius.angle = 3*M_PI_2;
  else if(x==0 && y==0)
  radius.angle = 0;
  radius.angle *= 180/M_PI;
  ArgList command;
  command.name = "mh";
  command.args.append(QString::number(round(radius.angle)));
  command.args.append(QString::number(radius.length));
  QString char_command = OTHER_H::makeCommand(command);

  MainWindow::sendCommand(char_command.toStdString().c_str());
}

void MainWindow::on_comboBox_joystik_currentTextChanged(const QString &arg1)
{
  if(! joystick.Initialize(arg1, 50) > 0)
    QMessageBox::warning(this, "Error", "Couldn't open joystick!");
}
