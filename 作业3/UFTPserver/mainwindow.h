#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QUdpSocket>
#include <QMainWindow>
#include"filelist.h"

#include"subthread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
extern filelist files;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QUdpSocket *main_socket;
    int next_sub_thread;
    //static filelist files;
public slots:
    void main_readdata();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
