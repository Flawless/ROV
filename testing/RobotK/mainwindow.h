#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtGui/QKeyEvent>
#include <QTime>

#include <joystickcontrol.h>
#include <other.h>
#include <core.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(Core* core, QWidget *parent = 0);
  ~MainWindow();
  /* QString currentJoystickName(); */
  /* int depth_manipulationType(); 
  /* int plain_manipulationType(); // 0 - desired coords(unused); 1 - manual (GUI Sliders); 2 - joystick */
private slots:
  void on_pushButton_connect_clicked();
  void on_pushButton_disconnect_clicked();
  /* void on_pushButton_sendCommand_clicked(); */
  /* void on_lineEdit_command_returnPressed(); */
  /* void on_pushButton_up_clicked(); */
  /* void on_pushButton_up_right_clicked(); */
  /* void on_pushButton_right_clicked(); */
  /* void on_pushButton_down_right_clicked(); */
  /* void on_pushButton_down_clicked(); */
  /* void on_pushButton_down_left_clicked(); */
  /* void on_pushButton_left_clicked(); */
  /* void on_pushButton_up_left_clicked(); */
  void on_groupBox_manual_toggled(bool arg1);
  void on_groupBox_joystick_toggled(bool arg1);
  void on_groupBox_buttons_toggled(bool arg1);
  /* void on_slider_x_valueChanged(int value); */
  /* void on_slider_z_valueChanged(int value); */
  /* void on_slider_y_valueChanged(int value); */
  /* void sendSpeedCommand(int x,int y); */
  void on_comboBox_joystik_currentTextChanged(const QString &arg1);
signals:
  void sig_joystickChanged(QString joystickName);
  void sig_connect(QString ip, int port);
  void sig_disconnect();
  void sig_positionControlTypeChanged(int controlType); // 1 - manual (GUI sliders); 2 - joystick
  void sig_depthControlTypeChanged(int controlType); // 0 - desired depth; 1 - manual (GUI sliders); 2 - joystick

public:

private:
  Ui::MainWindow *ui;
  QString qstringCommand;
  Core* pointer_Core;
};

#endif // MAINWINDOW_H
