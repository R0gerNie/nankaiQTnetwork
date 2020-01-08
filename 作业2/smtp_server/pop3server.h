#ifndef SERVER_H
#define SERVER_H
#include<QTcpSocket>
#include<QHostAddress>
#include<QTcpServer>
class pop3server
{
public:
    QTcpSocket* m_socket;
    QTcpServer* tcpServer;
    pop3server();
    void init();
    void receive();
public slots:
    void NewConnectionSlot();
    void printmail();
};

#endif // SERVER_H
