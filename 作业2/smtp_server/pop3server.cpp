#include "pop3server.h"

pop3server::pop3server()
{

}
void pop3server::init(){
    /*m_socket = new QTcpSocket;
    m_socket->bind(QHostAddress::LocalHost,7101);
    connect(m_socket,SIGNAL(readyRead()), this,SLOT(printmail()));*/
    tcpServer = new QTcpServer();


    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));


}
