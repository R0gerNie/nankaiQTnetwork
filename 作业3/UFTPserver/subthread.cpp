#include "subthread.h"
#include"mainwindow.h"
filelist files;
Subthread::Subthread()
{
    mode=0;
    stopped=false;
}

Subthread::Subthread(int port)
{
    mode=0;
    stopped=false;
    this->lport=port;
}

void Subthread::run(){
    this->sub_socket=new QUdpSocket;
    this->sub_socket->bind(QHostAddress::LocalHost,lport);
    connect(sub_socket, SIGNAL(readyRead()), this, SLOT(sub_readdata()));
    qDebug()<<"open run on"<<lport;
    QByteArray* sendbytes;
    while(!stopped){
        while(sub_socket->hasPendingDatagrams()){
            QByteArray buf;
            buf.resize(sub_socket->pendingDatagramSize());
            QHostAddress cip;
            quint16 cnport;
            sub_socket->readDatagram(buf.data(),buf.size(),&cip,&cnport);
            qDebug()<<"get data\n"<<cip<<" "<<cnport<<" "<<buf.size();
            QString recv=buf.data();
            if(QString::compare(recv,"get",Qt::CaseInsensitive)==0){
                //客户端回应以说明其收到了子进程端口号,所以服务器端应该返回一个filename
                QString rec="filelist";
                QByteArray recb=rec.toUtf8();
                qDebug()<<recb<<" "<<rec<<" "<<cip<<" "<<cport;
                sub_socket->writeDatagram(recb.data(),recb.size(),cip,this->cport);
                //sub_socket->writeDatagram(recb.data(),recb.size(),QHostAddress::LocalHost,this->cport);
                //接下来直接发送一个文件表，文件表是一个线程间共享的字符串数组
                QString fls=files.outfilelist();
                QByteArray flsb=fls.toUtf8();
                qDebug()<<flsb<<" "<<fls<<" "<<cip<<" "<<cport;
                sub_socket->writeDatagram(flsb.data(),flsb.size(),cip,this->cport);
                //sub_socket->writeDatagram(flsb.data(),flsb.size(),QHostAddress::LocalHost,this->cport);
            }
            else if(recv.toInt()>=0&&recv.toInt()<16&&mode==0){
                //选好了下载的文件的情形
                QFile file(files.filename[recv.toInt()]);
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QByteArray t = file.readAll();
                QString start="start,";
                start.append(QString::number(t.size()));
                QByteArray startb=start.toUtf8();
                sub_socket->writeDatagram(startb.data(),startb.size(),cip,this->cport);
                //sub_socket->writeDatagram(startb.data(),startb.size(),QHostAddress::LocalHost,this->cport);
                //转换到发送数据模式
                mode=1;
                qDebug()<<"chgmod to 1";
                int group=t.size()/1432+1;
                sendbytes=new QByteArray[group];
                for(int i=0;i<group;i++){
                    for(int j=0;j<1432;j++){
                        if(j+1432*i<t.size()){
                            sendbytes[i][j]=t[j+1432*i];
                        }
                    }
                }
                qDebug()<<t.size()<<" "<<group;
                //发第0个数据报
                //sub_socket->writeDatagram(sendbytes[0].data(),sendbytes[0].size(),cip,this->cport);
                //sub_socket->writeDatagram(sendbytes[0].data(),sendbytes[0].size(),QHostAddress::LocalHost,this->cport);
            }
            else if(mode==1){
                //来的请求是几，就发第几个数据报

                int k=recv.toInt();
                qDebug()<<k;

                    sub_socket->writeDatagram(sendbytes[k].data(),sendbytes[k].size(),cip,this->cport);


                //sub_socket->writeDatagram(sendbytes[k].data(),sendbytes[k].size(),QHostAddress::LocalHost,this->cport);
            }



        }
    }
    this->stopped=true;
}
void Subthread::stop(){
    stopped=true;
}
void Subthread::sub_readdata(){
    /* while(sub_socket->hasPendingDatagrams()){
         QByteArray buf;
         buf.resize(sub_socket->pendingDatagramSize());
         QHostAddress cip;
         quint16 cnport;
         sub_socket->readDatagram(buf.data(),buf.size(),&cip,&cnport);
         qDebug()<<"get data\n"<<cip<<" "<<cnport<<" "<<buf.size();
         QString recv=buf.data();
         if(QString::compare(recv,"get",Qt::CaseInsensitive)==0){
             //客户端回应以说明其收到了子进程端口号,所以服务器端应该返回一个filename
             QString rec="filelist";
             QByteArray recb=rec.toUtf8();
             qDebug()<<recb<<" "<<rec<<" "<<cip<<" "<<cport;
             sub_socket->writeDatagram(recb.data(),recb.size(),cip,this->cport);
             sub_socket->writeDatagram(recb.data(),recb.size(),QHostAddress::LocalHost,this->cport);
         }


     }
*/
}

