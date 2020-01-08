#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QNetworkDatagram>
#include<QDateTime>
#include<iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->recvtext->setText("receving...");
    r_socket=new QUdpSocket;
    r_socket->bind(QHostAddress::LocalHost,7001);
    connect(r_socket, SIGNAL(readyRead()), this, SLOT(readdata()));

    QHostAddress seadr=QHostAddress::LocalHost;
    //quint16 rport=7001;


    //cout<<"Helloworld"<<QHostAddress::LocalHost<<endl;

}
void MainWindow::readdata(){
    while(r_socket->hasPendingDatagrams()){
        qDebug()<<"get";
        QByteArray buf;
        QNetworkDatagram datagram = r_socket->receiveDatagram();
        buf.resize(r_socket->pendingDatagramSize());

        QHostAddress sip;
        quint16 sport;

        r_socket->readDatagram(buf.data(),buf.size(),&sip,&sport);
        QString recv=buf.data();
        quint32 cadr=sip.toIPv4Address();
        quint8 a=cadr/(65536*256);
        quint8 b=(cadr/65536)%256;
        quint8 c=(cadr/256)%256;
        quint8 d=cadr%256;
        QString cads=QString::number(a)+"."+QString::number(b)+"."+QString::number(c)+"."+QString::number(d);
        QString outs="Message "+recv+" from: "+cads+":"+QString::number(sport)+" received.";
        //cout<<"getdata:"<<recv.toStdString()<<"buf:"<<buf.toStdString()<<"sip"<<cads.toStdString()<<"sport"<<sport<<endl;
        //ui->recvtext->setText(recv.toLatin1().data());
        ui->recvtext->append(outs);
        //ui->recvtext->append("\n");
        //ABOVE! DO NOT CHANGE!



        QUdpSocket * a_socket=new QUdpSocket;
        if(QString::compare(recv,"date",Qt::CaseInsensitive)==0){
            //get a time back
            QDateTime current_date_time =QDateTime::currentDateTime();
            //QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
            QString current_date =current_date_time.toString("yyyy.MM.dd");
            QByteArray cdbyte=current_date.toUtf8();
            a_socket->writeDatagram(cdbyte.data(),cdbyte.size(),QHostAddress(cads),7002);
            //a_socket->writeDatagram(cdbyte.data(),cdbyte.size(),QHostAddress("192.168.195.131"),7002);
            a_socket->writeDatagram(cdbyte.data(),cdbyte.size(),QHostAddress::LocalHost,7002);
        }
        else if(QString::compare(recv,"time",Qt::CaseInsensitive)==0){
            //get a date back
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("hh:mm:ss");

            QByteArray cdbyte=current_date.toUtf8();
            a_socket->writeDatagram(cdbyte.data(),cdbyte.size(),QHostAddress(cads),7002);
            //a_socket->writeDatagram(cdbyte.data(),cdbyte.size(),QHostAddress("192.168.195.131"),7002);
            a_socket->writeDatagram(cdbyte.data(),cdbyte.size(),QHostAddress::LocalHost,7002);

        }
        else{
            //get error back
            QString eout ="Error";

            QByteArray cdbyte=eout.toUtf8();
            a_socket->writeDatagram(cdbyte.data(),cdbyte.size(),QHostAddress(cads),7002);
            a_socket->writeDatagram(cdbyte.data(),cdbyte.size(),QHostAddress::LocalHost,7002);

        }

    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

