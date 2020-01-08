#ifndef SUBTHREAD_H
#define SUBTHREAD_H

#include <QObject>
#include<QThread>
#include<QUdpSocket>

class Subthread:public QThread
{
    Q_OBJECT
public:
    Subthread();
    Subthread(int port);
    QUdpSocket* sub_socket;
    int mode;//交互为0，传输为1
    int lport;//子进程监听的端口；
    int cport;//与其交互的客户端口;
    void stop();
    void run() override;
    bool stopped;
public slots:
    void sub_readdata();
protected:

};

#endif // SUBTHREAD_H
