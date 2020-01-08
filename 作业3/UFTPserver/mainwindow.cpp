#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QNetworkDatagram>
//filelist files;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->recvtext->setText("waiting...");
    this->next_sub_thread=16000;
    main_socket=new QUdpSocket;
    main_socket->bind(QHostAddress::LocalHost,7005);
    connect(main_socket, SIGNAL(readyRead()), this, SLOT(main_readdata()));
}

void MainWindow::main_readdata(){
    while(main_socket->hasPendingDatagrams()){

            QByteArray buf;
            buf.resize(main_socket->pendingDatagramSize());
            QHostAddress cip;
            quint16 cport;
            int rport;
            main_socket->readDatagram(buf.data(),buf.size(),&cip,&cport);
            qDebug()<<"get data "<<cip<<" "<<cport<<" "<<buf.size();
            QString recv=buf.data();
            if(QString::compare(recv.mid(0,5),"hello",Qt::CaseInsensitive)==0){
                //这是第一次被访问时应当得到的信息，此时应当开子线程
                ui->pushButton->setEnabled(false);
                ui->pushButton_2->setEnabled(false);
                rport=recv.mid(6,buf.size()-6).toInt();
                Subthread* ath=new Subthread(next_sub_thread);
                ath->cport=rport;
                ath->start();
                QString rec=QString::number(next_sub_thread);
                ui->recvtext->append("a sub_thread opened and listening "+QString::number(next_sub_thread));
                next_sub_thread++;
                QByteArray recb=rec.toUtf8();
                qDebug()<<recb<<" "<<rec<<" "<<cip<<" "<<rport;
                main_socket->writeDatagram(recb.data(),recb.size(),cip,rport);
                //main_socket->writeDatagram(recb.data(),recb.size(),QHostAddress::LocalHost,rport);
            }
            else if(QString::compare(recv.mid(0,4),"done",Qt::CaseInsensitive)==0){
                //done发给主线程

                QString rec="bye";
                QByteArray recb=rec.toUtf8();
                qDebug()<<recb<<" "<<rec<<" "<<cip<<" "<<cport;
                main_socket->writeDatagram(recb.data(),recb.size(),cip,cport);
                ui->recvtext->append("a transmitting event finished.");

            }

    }


}




MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //files.addfilelist();
    QString file_name;
    file_name=QFileDialog::getOpenFileName(this,tr("Open file"),tr(""),tr("All files(*)"));
    files.filename[files.set]=file_name;
    //QFile tmpfile(file_name);
    //filein[set]=new QFile();
    //filein[set]->open()
    ui->filetext->append(QString::number(files.set));
    ui->filetext->append(file_name);
    ui->filetext->append("\n");
    files.set++;
}

void MainWindow::on_pushButton_2_clicked()
{
    files.clearfilelist();
    ui->filetext->clear();
}
