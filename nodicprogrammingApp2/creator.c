#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QDateTime>
#include <QProcess>
#include <mythread.h>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->progressBar->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 0) }");
    //ui->progressBar->setStyleSheet("QProgressBar{border: 1px solid grey;border-radius: 5px;text-align: center;}"
    //                         "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}");
    //ui->progressBar->setGeometry(100, 100, 150, 23);
    //app.seWindowIcon(QIcon("nodic.ico"));
    //setWindowFlags(Qt::WindowCloseButtonHint); //只要关闭按钮
    //setWindowFlags(Qt::FramelessWindowHint);//设置无边框
    //setAttribute(Qt::WA_TranslucentBackground, true);//设置背景透明
    this->setFixedSize( this->width(),this->height());//设置窗体固定大小，不能改变窗体大小
}

MainWindow::~MainWindow()
{
    delete ui;
}
//函 数 名：HexToAsc()
//功能描述：把16进制转换为ASCII
char MainWindow::IntToStr(char aChar)
{
    char ss;
    printf("%s %d\n",__FUNCTION__,aChar);
    switch(aChar)
    {
        case 0: ss= '0';break;
        case 1: ss= '1';break;
        case 2: ss= '2';break;
        case 3: ss= '3';break;
        case 4: ss= '4';break;
        case 5: ss= '5';break;
        case 6: ss= '6';break;
        case 7: ss= '7';break;
        case 8: ss= '8';break;
        case 9: ss= '9';break;

        case 10: ss= 'A';break;
        case 11: ss= 'B';break;
        case 12: ss= 'C';break;
        case 13: ss= 'D';break;
        case 14: ss= 'E';break;
        case 15: ss= 'F';break;
        default:break;
    }
    printf("%s %c\n",__FUNCTION__,ss);
    return ss;
}
//函 数 名：StrToInt()
//功能描述：把字符转换成对应的数字，比如a转换成10
char MainWindow::StrToInt(char aChar)
{
    char ss;
    printf("%s %c\n",__FUNCTION__,aChar);
    switch(aChar)
    {
        case '0': ss= 0;break;
        case '1': ss= 1;break;
        case '2': ss= 2;break;
        case '3': ss= 3;break;
        case '4': ss= 4;break;
        case '5': ss= 5;break;
        case '6': ss= 6;break;
        case '7': ss= 7;break;
        case '8': ss= 8;break;
        case '9': ss= 9;break;

        case 'A': ss= 10;break;
        case 'B': ss= 11;break;
        case 'C': ss= 12;break;
        case 'D': ss= 13;break;
        case 'E': ss= 14;break;
        case 'F': ss= 15;break;
        default:break;
    }
    printf("%s %d\n",__FUNCTION__,ss);
    return ss;
}
void MainWindow::on_pushButton_clicked()
{
    QByteArray readCmd;
    QByteArray readCm;
    QByteArray readCmdMac;
    char readCharMac;
    static unsigned long erase=0;
    int i=0;
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::green);
    ui->label->setPalette(pa);
    ui->label->setText("开始...");
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);

    for(int s=0;i<5;i++)
    {
        bool R=false;
        qDebug("%d",rand());
        QFile file("weiqifa.txt");
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            qDebug()<<"Can't open the file!"<<endl;
        }
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            qDebug()<<str;
            if(str.compare("weiqifa")==0)
            {
                qDebug()<<"ok.................."<<endl;
                //return;
            }
            else
            {
                qDebug()<<"error.................."<<endl;
                if((rand()%9)==0)
                {
                    QMessageBox::information(this,"!","Error!");
                    R=true;
                    return;
                }
            }
        }
        if(R==true)
        {
            qDebug()<<"return.................."<<endl;
            return;
        }
    }
    //执行cmd的相关命令
    QProcess p(0);
    //擦除
    p.start(ERASE_ALL_CMD);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    //提示
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("擦除失败");
        return;
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("擦除成功");
    }
    MyThread thread;
    thread.start();
    int thrCount=0;
    ui->progressBar->setRange(0,5000);
    //ui->progressBar->setModal(true);
    do
    {
        QCoreApplication::processEvents();/*Don't move it*/
        thrCount++;
        ui->progressBar->setValue(thrCount);
        //msleep(1000);
        QThread::usleep(3000);
        //ui->textBrowser->append(tr("等待")+tr("[%1]").arg(thrCount)+tr("次"));
    }while(thread.stop==false);
    thread.stop=false;
    thread.quit();
    ui->progressBar->setValue(thrCount+(5000-thrCount)/2);

    //读取MAC地址
    p.start(READ_MAC_CMD);
    p.waitForStarted();
    p.waitForFinished();
    //获取MAC地址
    readCm  = p.readAllStandardOutput().trimmed();
    qDebug()<<readCm<<endl;
    //复位 如果不复位的话，板子现在在烧录模式，灯还一直亮着
    //复位
    p.start(RESET_CMD);
    p.waitForStarted();
    p.waitForFinished();
    readCmd="";
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);

    //提示
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("复位失败");
        return;
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("烧录成功");
    }
    qDebug()<<readCm;
    p.close();
    //判断数据是否正确，不正确就返回
    if(readCm.size()<5)
    {
         ui->textBrowser->append(readCmdMac);
         ui->textBrowser->append(tr("1查看是否安装了jlink驱动，连接了jlink并连接了设备......."));
         return;
    }

    readCmd = readCm.mid(15,17);
    //去掉空格
    readCmdMac.resize(12);
    for(int j=0;j<readCmd.size();j++)
    {
        if(readCmd.at(j)!=' ')
        {
            readCmdMac[i++]=(readCmd.at(j));
        }
    }
    readCmdMac[i]='\0';
    //把最后一个字节的那个位改变一下
    //qDebug("%x",readCmdMac.data()[10]);
    readCharMac=readCmdMac.data()[10];
    readCharMac=StrToInt(readCharMac);
    //qDebug("%x",readCharMac);
    readCharMac|=0xC;
    //qDebug("%x",readCharMac);
    readCmdMac.data()[10]=IntToStr(readCharMac);
    //qDebug()<<readCmdMac.data()[10];
    qDebug()<<readCmdMac;
    qDebug("%d",readCmdMac.size());
    if(readCmdMac.size()<13)
    {
        ui->textBrowser->append(readCmdMac);
        ui->textBrowser->append(tr("2查看是否安装了jlink驱动，连接了jlink并连接了设备......."));
        return;
    }

    //把mac地址保存到文件里面
    QFile file("macAdress.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        QTextStream stream( &file );
        stream << readCmdMac << "\r\n";
        file.close();
        ui->textBrowser->append(tr("保存MAC地址成功..."));
    }
    else
    {
        ui->textBrowser->append(tr("打开文件失败..."));
    }
    erase++;
    QString s;
    ui->progressBar->setValue(5000);
    ui->progressBar->setVisible(false);
    ui->textBrowser->append(tr("烧录成功第")+tr("[%1]").arg(erase)+tr("次")+tr("MAC")+tr("[%1]").arg(s.append(readCmdMac)));
}

void MainWindow::on_pushButton_2_clicked()
{
    QByteArray readCmd;
    QProcess p(0);
    p.start(ALL_IN_ONE_CMD_1);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);

    readCmd="";
    p.start(ALL_IN_ONE_CMD_2);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);

    readCmd="";
    p.start("cmd.exe");
    p.write(DEL_TEMP);
    p.write ("exit\n\r");
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    p.close();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
    ui->textBrowser->setText("1、安装了nRF5x-Command-Line-Tools_8_4_0_Installer.exe\r\n\
2、C:\\Program Files\\NordicSemiconductor\\nrf5x\bin\\nrfjprog.ini 中Family改为NRF52\r\n\
3、ts102.hex放在与本该程序相同的目录下\r\n\
4、存放的路径不能有中文，桌面也不行\r\n\
5、合并文件时把，nrf52832_xxaa_s132_bootloader.hex s132_nrf52_2.0.0_softdevice.hex nrf52832_xxaa_s132.hex 放在此exe同一个目录下\r\n\
6、联系QQ：329410527 ");
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::black);
    ui->label->setPalette(pa);
    ui->label->setText("烧录结果");
}
