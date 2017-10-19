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
    updateReadCmd = readCmd;
    stop=true;
    qDebug()<< stop;
}
