#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtGui/QKeyEvent>

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

signals:
private:
    Ui::MainWindow *ui;
    QTcpSocket socket;
};

#endif // MAINWINDOW_H
