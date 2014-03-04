#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "joystickcontrol.h"
#include "other.h"
#include "math.h"

#include <QtWidgets/QMessageBox>
#include <QTime>
#include <QStringList>

MainWindow::MainWindow(Core* pCore, QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  pointer_Core = pCore;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
  emit sig_connect(ui->lineEdit_ip->text(), ui->lineEdit_portNumber->text().toInt());
}
void MainWindow::on_pushButton_disconnect_clicked()
{
  emit sig_disconnect();
}
void MainWindow::on_groupBox_manual_toggled(bool arg1)
{
  if(arg1) {
    ui->groupBox_buttons->setChecked(!arg1);
    ui->groupBox_joystick->setChecked(!arg1);
    emit sig_positionControlTypeChanged(1);
  }
}
void MainWindow::on_groupBox_joystick_toggled(bool arg1)
{
  if(arg1) {
    ui->groupBox_buttons->setChecked(!arg1);
    ui->groupBox_manual->setChecked(!arg1);
    emit sig_positionControlTypeChanged(2);
  }
}
void MainWindow::on_groupBox_buttons_toggled(bool arg1)
{
  if(arg1) {
    ui->groupBox_manual->setChecked(!arg1);
    ui->groupBox_joystick->setChecked(!arg1);
    emit sig_positionControlTypeChanged(0);
  }
}
void MainWindow::on_comboBox_joystik_currentTextChanged(const QString &arg1)
{
  emit sig_joystickChanged(arg1);
}

// void MainWindow::on_pushButton_sendCommand_clicked()
// {
//   send(ui->lineEdit_command->text());
// }
// void MainWindow::on_lineEdit_command_returnPressed()
// {
//   on_pushButton_sendCommand_clicked();
// }

// void MainWindow::on_pushButton_up_clicked()
// {
//   send("#mh.0." + QString::number((ui->spinBox_power->value())) + "!");
// }

// void MainWindow::on_pushButton_up_right_clicked()
// {
//   send("#mh.45." + QString::number((ui->spinBox_power->value())) + "!");
// }

// void MainWindow::on_pushButton_right_clicked()
// {
//   send("#mh.90." + QString::number((ui->spinBox_power->value())) + "!");
// }

// void MainWindow::on_pushButton_down_right_clicked()
// {
//   send("#mh.135." + QString::number((ui->spinBox_power->value())) + "!");
// }

// void MainWindow::on_pushButton_down_clicked()
// {
//   send("#mh.180." + QString::number((ui->spinBox_power->value())) + "!");
// }

// void MainWindow::on_pushButton_down_left_clicked()
// {
//   send("#mh.225." + QString::number((ui->spinBox_power->value())) + "!");
// }

// void MainWindow::on_pushButton_left_clicked()
// {
//   send("#mh.270." + QString::number((ui->spinBox_power->value())) + "!");
// }

// void MainWindow::on_pushButton_up_left_clicked()
// {
//   send("#mh.315." + QString::number((ui->spinBox_power->value())) + "!");
// }

// void MainWindow::on_slider_x_valueChanged(int value)
// {
//   sendSpeedCommand(ui->slider_x->value(), ui->slider_y->value());

// }

// void MainWindow::on_slider_z_valueChanged(int value)
// {

// }

// void MainWindow::on_slider_y_valueChanged(int value)
// {
//   sendSpeedCommand(ui->slider_x->value(), ui->slider_y->value());
// }

// void MainWindow::sendSpeedCommand(int x, int y)
// {
//   Radius radius;
//   radius.length = sqrt(y*y + x*x);
//   if(x>0 && y>=0)
//     radius.angle = atan(y/x);
//   else if(x>0 && y<0)
//     radius.angle = atan(y/x) + 2*M_PI;
//   else if(x<0)
//     radius.angle = atan(y/x) + M_PI;
//   else if(x==0 && y>0)
//     radius.angle = M_PI_2;
//   else if(x==0 && y>0)
//     radius.angle = 3*M_PI_2;
//   else if(x==0 && y==0)
//     radius.angle = 0;
//   radius.angle *= 180/M_PI;
//   command.name = "mh";
//   command.args.append(QString::number(round(radius.angle)));
//   command.args.append(QString::number(radius.length));
// //  QString char_command = OTHER_H::makeCommand(command);
//   sendCommand(command, true);
// //  MainWindow::sendCommand(char_command.toStdString().c_str());
// }


