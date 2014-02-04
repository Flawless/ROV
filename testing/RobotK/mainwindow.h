#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtGui/QKeyEvent>

#include <joystickcontrol.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void joystick_axisChanged(int arg1, int arg2, int arg3);
  void socked_connected();
  void socked_disconnected();
  void on_pushButton_connect_clicked();
  //Working with commands
  void sendCommand(QString command);
  //    bool checkCommand(char command);

  void on_pushButton_sendCommand_clicked();

  void on_lineEdit_command_returnPressed();

  void on_pushButton_up_clicked();

  void on_pushButton_up_right_clicked();

  void on_pushButton_right_clicked();

  void on_pushButton_down_right_clicked();

  void on_pushButton_down_clicked();

  void on_pushButton_down_left_clicked();

  void on_pushButton_left_clicked();

  void on_pushButton_up_left_clicked();

  void on_groupBox_manual_toggled(bool arg1);

  void on_groupBox_joystick_toggled(bool arg1);

  void on_groupBox_buttons_toggled(bool arg1);

  void on_slider_x_valueChanged(int value);

  void on_slider_z_valueChanged(int value);

  void on_slider_y_valueChanged(int value);

  void sendSpeedCommand(int x,int y);

  void on_comboBox_joystik_currentTextChanged(const QString &arg1);

signals:
private:
  Ui::MainWindow *ui;
  QTcpSocket socket;
  JoystickControl joystick;
  bool joystickEnabled,
          initialized;
};

#endif // MAINWINDOW_H
