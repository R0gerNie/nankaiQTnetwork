#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<iostream>
#include<QNetworkDatagram>
#include<QDateTime>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    b_socket=new QUdpSocket;
    b_socket->bind(QHostAddress::LocalHost,7002);
    connect(b_socket, SIGNAL(readyRead()), this, SLOT(retdata()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //Now start listens!

    QString message=ui->messText->toPlainText();
    QByteArray mesbyte=message.toUtf8();
    QString sip=ui->ipEdit->text();
    QHostAddress adr=QHostAddress(sip.toStdString().data());
    QString sport=ui->portEdit->text();
    //qint16 sp=sport.toInt();
    quint16 sp=7001;
    //get the plain message
    cout<<mesbyte.data()<<sip.toLatin1().data()<<sp<<endl;
    QUdpSocket * s_socket=new QUdpSocket;
    s_socket->writeDatagram(mesbyte.data(),mesbyte.size(),adr,sport.toInt());//NULL MESSAGE? Why?但是1，3两条同时开又可以正常发送
    //s_socket->writeDatagram(mesbyte.data(),mesbyte.size(),QHostAddress("192.168.195.131"),sport.toInt());
    //s_socket->writeDatagram(mesbyte.data(),mesbyte.size(),QHostAddress::LocalHost,sport.toInt());
    /*s_socket->bind(QHostAddress::LocalHost, 7001);
    connect(s_socket, SIGNAL(readyRead()), this, SLOT(receive()));*/
    //got the port


}
void MainWindow::retdata(){
    //the function which print to the textback after sending
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString curdate =current_date_time.toString("yyyy.MM.dd hh:mm:ss");
    cout<<"open Receive"<<endl;
    while(b_socket->hasPendingDatagrams()){
        QByteArray buf;
        QNetworkDatagram datagram = b_socket->receiveDatagram();
        buf.resize(b_socket->pendingDatagramSize());

        QHostAddress sip;
        quint16 sport;

        b_socket->readDatagram(buf.data(),buf.size(),&sip,&sport);
        QString recv=buf.data();
        quint32 cadr=sip.toIPv4Address();
        quint8 a=cadr/(65536*256);
        quint8 b=(cadr/65536)%256;
        quint8 c=(cadr/256)%256;
        quint8 d=cadr%256;
        QString cads=QString::number(a)+"."+QString::number(b)+"."+QString::number(c)+"."+QString::number(d);
        QString outs=" "+curdate+" from server: "+cads+":"+QString::number(sport)+" send:" +ui->messText->toPlainText()+" and received \""+recv+"\".";
        qDebug()<<outs<<endl;
        ui->textback->append(outs);
    }


}
