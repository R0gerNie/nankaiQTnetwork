#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ctime>
#include<windows.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mode=0;
    qsrand(time(NULL));
    port = qrand() % 1024+6400;
    cli_socket=new QUdpSocket;
    cli_socket->bind(QHostAddress::LocalHost,port);
    connect(cli_socket, SIGNAL(readyRead()), this, SLOT(recvdata()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::recvdata(){
    while(cli_socket->hasPendingDatagrams()){
        QByteArray buf;
        buf.resize(cli_socket->pendingDatagramSize());
        QHostAddress srip;
        quint16 srport;
        cli_socket->readDatagram(buf.data(),buf.size(),&srip,&srport);
        QString recv=buf.data();
        if(recv.size()<=64||mode==1){
            ui->action->append("received:");
            ui->action->append(recv);
        }
        else{
            ui->action->append("received:");
            ui->action->append(recv.mid(0,63)+"...");
            //ui->action->append("...\n");
        }
        bool ok;
        int p=recv.toInt(&ok);
        if(mode==0&&ok==true&&p>0&&p<65535){
            //返回的是一个端口，这个值即为服务器端子线程监听的端口
            this->subport=p;
            ui->action->append("open a port of "+QString::number(p));
            ui->action->append("\n");

            QString seip=ui->ip->text();
            QHostAddress sip=QHostAddress(seip.toStdString().data());
            QString mes="get";
            QByteArray mesb=mes.toUtf8();
            cli_socket->writeDatagram(mesb.data(),mesb.size(),sip,subport);
        }
        else if(mode==0&&QString::compare(recv.mid(0,8),"filelist",Qt::CaseInsensitive)==0){
            //收到filelist，接下来会直接收到一个文件表
            mode=1;
            qDebug()<<"chgmod to 1";
        }
        else if(mode==1&&QString::compare(recv.mid(0,5),"start",Qt::CaseInsensitive)==0){
            //准备接收数据的情况
            filsize=recv.mid(6,recv.size()-6).toInt();
            int group=filsize/1432+1;
            sendbytes=new QByteArray[group];
            mode=2;
            qDebug()<<"chgmod to 2";
            au=0;
            ack=0;
            //发一个0过去就能接收第0个数据包
            QString message=QString::number(au);
            QByteArray mesbyte=message.toUtf8();
            QString sip=ui->ip->text();
            QHostAddress adr=QHostAddress(sip.toStdString().data());
            int sport=subport;
            cli_socket->writeDatagram(mesbyte.data(),mesbyte.size(),adr,sport);

        }
        else if(mode==2){
            //把数据抓到sendbytes里
            QHostAddress srip;
            quint16 srport;
            sendbytes[au]=buf;
            qDebug()<<ack<<" "<<au<<" "<<sendbytes[au].size();
            if(sendbytes[au].size()!=0){
                ack+=sendbytes[au].size();
                au++;
                qDebug()<<ack<<" "<<au;
            }

            if(ack>=filsize){
                //全部收到，回一个done
                QString message="done";
                QByteArray mesbyte=message.toUtf8();
                QString sip=ui->ip->text();
                QHostAddress adr=QHostAddress(sip.toStdString().data());
                QString hport=ui->port->text();
                cli_socket->writeDatagram(mesbyte.data(),mesbyte.size(),adr,hport.toInt());
                mode=3;
            }
            else{
                //申请下一个数据包
                //Sleep(10000);//助教提问时，增加该函数来证明同时传输
                QString message=QString::number(au);
                QByteArray mesbyte=message.toUtf8();
                QString sip=ui->ip->text();
                QHostAddress adr=QHostAddress(sip.toStdString().data());
                int sport=subport;
                cli_socket->writeDatagram(mesbyte.data(),mesbyte.size(),adr,sport);
            }
        }
        else if(mode==3&&QString::compare(recv.mid(0,3),"bye",Qt::CaseInsensitive)==0){
            for(int i=0;i<ack/1432+1;i++){
                ui->fileim->append(this->sendbytes[i].data());
            }
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
     QString message="hello,";
     message.append(QString::number(port));
     QByteArray mesbyte=message.toUtf8();
     QString sip=ui->ip->text();
     QHostAddress adr=QHostAddress(sip.toStdString().data());
     QString sport=ui->port->text();
     QUdpSocket * s_socket=new QUdpSocket;
     s_socket->writeDatagram(mesbyte.data(),mesbyte.size(),adr,sport.toInt());

}

void MainWindow::on_pushButton_3_clicked()
{
    QString message=ui->message->text();
    ui->action->append("You choosed the file of number "+message);
    QByteArray mesbyte=message.toUtf8();
    QString sip=ui->ip->text();
    QHostAddress adr=QHostAddress(sip.toStdString().data());
    int sport=subport;
    QUdpSocket * s_socket=new QUdpSocket;
    s_socket->writeDatagram(mesbyte.data(),mesbyte.size(),adr,sport);
}

void MainWindow::on_pushButton_2_clicked()
{
    QByteArray sum;
    for(int i=0;i<ack/1432+1;i++){
        sum.append(this->sendbytes[i].data());
    }
    QString dir = QFileDialog::getSaveFileName(this,tr("Save file"),tr(""),tr("All files(*)"));
    QFile filename(dir);
    filename.open(QIODevice::WriteOnly);
    filename.write(sum);

}
