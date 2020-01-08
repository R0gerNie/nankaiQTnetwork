#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include<QHostAddress>
#include<QTcpServer>
#include <QFileDialog>
#include<QFile>
#include"base64.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpSocket* m_socket;
    QTcpServer* tcpServer;

    void init();

public slots:
    void NewConnectionSlot();
    void printmail();
    void disconnectedSlot();
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
