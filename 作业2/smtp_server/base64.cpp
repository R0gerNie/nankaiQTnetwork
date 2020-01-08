#include"base64.h"
QString encode(QString plain){
    //get a base 64 encode for cipher
    int type=0;
    if(plain.length()%3==1){
        type=2;//add two =
    }
    else if(plain.length()%3==2){
        type=1;//add one =
    }

    char b64table[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    QString cipher="";
    for(int i=0;i<plain.length();i+=3){
        QString temp=plain.mid(i,3);
        //use a math way to get four cipher chars
        QByteArray bte=temp.toUtf8();
        char* k=bte.data();
        QByteArray tre;
        tre.resize(4);
        tre[0]=b64table[k[0]>>2];
        tre[1]=b64table[(k[0]%4)*16+(k[1]>>4)];
        if(i>plain.length()-3){
            if(type>=2){
                tre[2]='=';
                tre[3]='=';
            }
            else{
                tre[2]=b64table[(k[1]%16)*4+(k[2]>>6)];
                if(type>=1){
                    tre[3]='=';
                }
                else{
                    tre[3]=b64table[k[2]%64];
                }
            }
        }
        else{
            tre[2]=b64table[(k[1]%16)*4+(k[2]>>6)];
            tre[3]=b64table[k[2]%64];
        }
        cipher.append(tre);
    }
    return cipher;
}
QString decode(QString cipher){
    if(cipher.length()%4!=0){
        return "error";
    }
    QString b64table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    QString plain="";
    for(int i=0;i<cipher.length();i+=4){
        QString temp=cipher.mid(i,4);
        QByteArray bte=temp.toUtf8();
        char*k=bte.data();
        QByteArray pla;
        pla.resize(3);
        pla[0]=b64table.indexOf(k[0])*4+(b64table.indexOf(k[1])>>4);
        if(k[2]=='='){
            pla.resize(1);
        }
        else{
            pla[1]=(b64table.indexOf(k[1])%16)*16+(b64table.indexOf(k[2])>>2);
            if(k[3]=='='){
                pla.resize(2);
            }
            else{
                pla[2]=(b64table.indexOf(k[2])%4*64)+b64table.indexOf(k[3]);
            }
        }
        plain.append(pla);

    }

    return plain;
}

QByteArray bdecode(QByteArray cipher){
    if(cipher.length()%4!=0){
        return "error";
    }
    QByteArray b64table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    QByteArray plain="";
    for(int i=0;i<cipher.length();i+=4){
        QByteArray temp=cipher.mid(i,4);
        QByteArray bte=temp;
        char*k=bte.data();
        QByteArray pla;
        pla.resize(3);
        pla[0]=(b64table.indexOf(k[0])<<2)+(b64table.indexOf(k[1])>>4);
        if(k[2]=='='){
            pla.resize(1);
        }
        else{
            pla[1]=((b64table.indexOf(k[1])%16)<<4)+(b64table.indexOf(k[2])>>2);
            if(k[3]=='='){
                pla.resize(2);
            }
            else{
                pla[2]=((b64table.indexOf(k[2])%4<<6))+(b64table.indexOf(k[3]));
            }
        }
        plain.append(pla);

    }

    return plain;
}
