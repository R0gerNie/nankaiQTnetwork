#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"pop3server.h"
#include <QtNetwork>

#include<iostream>

using namespace std;
int status;
int sizem;
QByteArray temp;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}
void MainWindow::init(){
    tcpServer = new QTcpServer(this);
    ui->textBrowser->append(QNetworkInterface().allAddresses().at(0).toString());   //获取本地IP
    ui->textBrowser->setText("Receving...\n");
    tcpServer->waitForNewConnection();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
    //m_socket=tcpServer->nextPendingConnection();
    tcpServer->listen(QHostAddress::Any,25);
    status=0;
    sizem=0;

}

MainWindow::~MainWindow()
{
    tcpServer->close();
    delete ui;
}
void MainWindow::NewConnectionSlot()
{
        m_socket = tcpServer->nextPendingConnection();

connect(m_socket, SIGNAL(readyRead()), this, SLOT(printmail()));


        ui->pushButton->setEnabled(true);
        ui->textBrowser->append("New Connection detected.");
        m_socket->write("220 Ready 127.0.0.1\r\n");
        m_socket->flush();
        ui->textBrowser->append("S:220 Ready 127.0.0.1\r\n");
        status++;
        temp=QByteArray("");
        temp.append(m_socket->readAll());
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
        cout<<temp.data()<<status<<endl;




}

void MainWindow::printmail(){


        temp.append(m_socket->readAll());
        cout<<temp.data()<<status<<endl;
        if(status==1){
            QString stemp(temp);
            if(QString::compare("EHLO",stemp.mid(0,4))==0){
                ui->textBrowser->append("C:"+stemp);
                QString mes="250 OK "+stemp.mid(5,16);
                m_socket->write(mes.toUtf8());
                     m_socket->flush();
                ui->textBrowser->append("S:250 OK "+stemp.mid(5,16));
                status=2;
                temp.clear();
                //应答EHLO
            }

        }
        else if(status==2){
            QString stemp(temp);
            if(QString::compare("MAIL FROM:",stemp.mid(0,10))==0){
                ui->textBrowser->append("C:"+stemp);
                m_socket->write("250 Sender OK\r\n");
                     m_socket->flush();
                ui->textBrowser->append("S:250 Sender OK\r\n");
                status=3;
                temp.clear();
                //应答from
            }


        }
        else if(status==3){
            QString stemp(temp);
            if(QString::compare("RCPT TO:",stemp.mid(0,8))==0){
                ui->textBrowser->append("C:"+stemp);
                m_socket->write("250 Receiver OK\r\n");
                     m_socket->flush();
                ui->textBrowser->append("S:250 Receiver OK\r\n");
                status=4;
                //应答to
                temp.clear();
            }


        }
        else if(status==4){
            QString stemp(temp);
            if(QString::compare("DATA",stemp.mid(0,4))==0){
                ui->textBrowser->append("C:"+stemp);
                m_socket->write("354 Go ahead. End with <CRLF>.<CRLF>\r\n");
                ui->textBrowser->append("S:354 Strat Mail. End with CRLF.CRLF\r\n");
                status=5;
                temp.clear();
                //应答data
            }


        }
        else if(status==5){

            /*
            if(sizeof(temp)>4){
                ui->textBrowser_2->setText(temp.data());
            }
            QDateTime t=QDateTime::currentDateTime();
            QString sender("");
            for(int i=0;i<128;i++){
                if(QString::compare(temp.mid(i,5),"From:")==0){
                    for(int j=i+6;j<i+32;j++){
                        if(QString::compare(temp.mid(j,1),"<")==0){
                            int len=j-(i+6);
                            sender=temp.mid(i+6,len-1);
                            break;
                        }
                    }
                break;
                }

            }
    //ui->textBrowser->append("C:");
            //ui->textBrowser->append("C:DATA");
            QString out("received mail from " + sender +" at "+t.toString("hh:mm:ss"));
            cout<<out.toUtf8().data();
            sizem+=sizeof(temp.data());
        //m_socket->close();
            //ui->textBrowser->append("250 OK , completed");
            //m_socket->write("250 OK , completed");
            //m_socket->flush();*/



    //ui->textBrowser->append("C:");


        //m_socket->close();

            QString stemp(temp);
            QString flag="\r\n.\r\n";
            if(stemp.indexOf(flag)!=-1){
                temp.resize(temp.size()-3);
                ui->textBrowser_2->setText(QString(temp.data()));
                ui->textBrowser->append("S:250 OK ,completed\r\n");
                status=6;
                temp.clear();
                //ui->textBrowser->append(stemp);
                m_socket->write("250 OK , completed\r\n");
                     m_socket->flush();

                //应答.
                temp.clear();
            }

            //m_socket->close();
        }
        else if(status==6){
            QString stemp(temp);
            if(QString::compare("QUIT",stemp.mid(0,4))==0){
                ui->textBrowser->append("C:"+stemp);
                m_socket->write("221 QUIT");
                ui->textBrowser->append("S:221 QUIT");
                status=7;
                m_socket->close();
                temp.clear();
                //应答quit
            }


        }


}
void MainWindow::disconnectedSlot(){
    status=0;
ui->textBrowser->append("connect closed.");
}

void MainWindow::on_pushButton_clicked()
{
    QString text=ui->textBrowser_2->toPlainText();
    int a=text.indexOf("filename=\"");
    int b=a+10;
    QString filename="";
    for(int i=0;;i++){
        QString m=text.mid(b+i,1);
        if(QString::compare("\"",m.mid(0,1))==0) {
            filename=text.mid(b,i);
            b=b+i+3;
            break;
        }//这里，b的位次是附件base64的开头
    }
    QString cip="";
    for(int i=1;;i++){
        QString c=text.mid(b,i);
        if(c.back()=="-"){
            c.resize(c.size());
            cip=c;
            break;
        }

    }
    //QFileDialog()
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream wr(&file);
    QByteArray byteout(cip.toUtf8());
    QByteArray bebased="";
    for(int i=0;byteout[i]!='-';i++){

            if(text.mid(a-128,192).indexOf("Content-Transfer-Encoding: base64")!=-1 or filename.indexOf(".bmp")!=-1 or filename.indexOf(".png")!=-1 or filename.indexOf(".gif")!=-1 or filename.indexOf(".jpg")!=-1 or  filename.indexOf(".zip")!=-1 or cip.mid(0,4).indexOf("UEsD")!=-1 ){
                //属于以下几种情况时解base64，有提及的，扩展名是png,jpg,gif,zip的，文件头是zip系的（word，ppt都是压缩包）
                if(byteout[i]!='\n'&&byteout[i]!=' '){
                    bebased.append(byteout[i]);
                }
            }
            else{
                wr<<byteout[i];
            }

    }
    if(text.mid(a-128,192).indexOf("Content-Transfer-Encoding: base64")!=-1  or filename.indexOf(".bmp")!=-1 or filename.indexOf(".png")!=-1 or filename.indexOf(".gif")!=-1 or filename.indexOf(".jpg")!=-1 or  filename.indexOf(".zip")!=-1 or cip.mid(0,4).indexOf("UEsD")!=-1){

        QByteArray bbp=bdecode(bebased);
        file.write(bbp);
    }
    QString aim="explorer "+filename;
    QProcess::execute(aim);

}
