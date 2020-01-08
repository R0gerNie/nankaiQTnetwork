#ifndef FILELIST_H
#define FILELIST_H
#include<QObject>
#include<QFile>
#include<QFileDialog>
class filelist
{
public:
    int set;
    QString filename[16];
    //QFile* filein[16];
    filelist();
    QString outfilelist();

    void clearfilelist();
};


#endif // FILELIST_H
