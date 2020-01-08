#include "filelist.h"

filelist::filelist()
{
    set=0;
}
QString filelist::outfilelist(){
    QString a="";
    for(int i=0;i<set;i++){
        a.append(QString::number(i));
        a.append(" ");
        a.append(this->filename[i]);
        a.append("\n");
    }
    return a;
}

void filelist::clearfilelist(){
    for(int i=0;i<16;i++){
        this->filename[i]="";
        //this->filein[i]=nullptr;

    }

}
