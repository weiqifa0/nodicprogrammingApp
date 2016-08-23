#include "mythread.h"
#include "mainwindow.h"
#include<QDebug>
#include <QProcess>
MyThread::MyThread(QObject *parent) :
    QThread(parent)
{
    stop = false;
}
void MyThread::run()
{
    QByteArray readCmd;
    QProcess p;
    p.start(DOWNLOAD_CMD);
    p.waitForStarted();
    p.waitForFinished();
    readCmd="";
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    qDebug()<< readCmd;
    stop=true;
    qDebug()<< stop;
//    for(int i=0;i<1000;i++)
//    {
//        if(stop)break;
//        qDebug()<<i;
//        QThread::sleep(1);
//    }
}
