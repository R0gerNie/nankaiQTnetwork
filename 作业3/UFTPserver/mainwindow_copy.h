#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QUdpSocket>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QUdpSocket *main_socket;
    int next_sub_thread;
public slots:
    void main_readdata();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
