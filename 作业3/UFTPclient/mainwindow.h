#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QUdpSocket>
#include<QFileDialog>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int subport;
    int mode;//0=交互,1=准备接收，2=正在接收,3=接收完
    int port;//本客户端监听的端口
    int ack;
    int au;
    int filsize;
    QUdpSocket* cli_socket;
    QByteArray* sendbytes;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void recvdata();
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
