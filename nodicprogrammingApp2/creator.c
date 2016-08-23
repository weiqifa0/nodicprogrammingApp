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
    //setWindowFlags(Qt::WindowCloseButtonHint); //ֻҪ�رհ�ť
    //setWindowFlags(Qt::FramelessWindowHint);//�����ޱ߿�
    //setAttribute(Qt::WA_TranslucentBackground, true);//���ñ���͸��
    this->setFixedSize( this->width(),this->height());//���ô���̶���С�����ܸı䴰���С
}

MainWindow::~MainWindow()
{
    delete ui;
}
//�� �� ����HexToAsc()
//������������16����ת��ΪASCII
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
//�� �� ����StrToInt()
//�������������ַ�ת���ɶ�Ӧ�����֣�����aת����10
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
    ui->label->setText("��ʼ...");
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
    //ִ��cmd���������
    QProcess p(0);
    //����
    p.start(ERASE_ALL_CMD);
    p.waitForStarted();
    p.waitForFinished();
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);
    //��ʾ
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("����ʧ��");
        return;
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("�����ɹ�");
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
        //ui->textBrowser->append(tr("�ȴ�")+tr("[%1]").arg(thrCount)+tr("��"));
    }while(thread.stop==false);
    thread.stop=false;
    thread.quit();
    ui->progressBar->setValue(thrCount+(5000-thrCount)/2);

    //��ȡMAC��ַ
    p.start(READ_MAC_CMD);
    p.waitForStarted();
    p.waitForFinished();
    //��ȡMAC��ַ
    readCm  = p.readAllStandardOutput().trimmed();
    qDebug()<<readCm<<endl;
    //��λ �������λ�Ļ���������������¼ģʽ���ƻ�һֱ����
    //��λ
    p.start(RESET_CMD);
    p.waitForStarted();
    p.waitForFinished();
    readCmd="";
    readCmd = p.readAllStandardOutput();
    readCmd+=p.readAllStandardError();
    ui->textBrowser->append(readCmd);

    //��ʾ
    if(readCmd.contains("ERROR"))
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        ui->label->setPalette(pa);
        ui->label->setText("��λʧ��");
        return;
    }
    else
    {
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::green);
        ui->label->setPalette(pa);
        ui->label->setText("��¼�ɹ�");
    }
    qDebug()<<readCm;
    p.close();
    //�ж������Ƿ���ȷ������ȷ�ͷ���
    if(readCm.size()<5)
    {
         ui->textBrowser->append(readCmdMac);
         ui->textBrowser->append(tr("1�鿴�Ƿ�װ��jlink������������jlink���������豸......."));
         return;
    }

    readCmd = readCm.mid(15,17);
    //ȥ���ո�
    readCmdMac.resize(12);
    for(int j=0;j<readCmd.size();j++)
    {
        if(readCmd.at(j)!=' ')
        {
            readCmdMac[i++]=(readCmd.at(j));
        }
    }
    readCmdMac[i]='\0';
    //�����һ���ֽڵ��Ǹ�λ�ı�һ��
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
        ui->textBrowser->append(tr("2�鿴�Ƿ�װ��jlink������������jlink���������豸......."));
        return;
    }

    //��mac��ַ���浽�ļ�����
    QFile file("macAdress.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        QTextStream stream( &file );
        stream << readCmdMac << "\r\n";
        file.close();
        ui->textBrowser->append(tr("����MAC��ַ�ɹ�..."));
    }
    else
    {
        ui->textBrowser->append(tr("���ļ�ʧ��..."));
    }
    erase++;
    QString s;
    ui->progressBar->setValue(5000);
    ui->progressBar->setVisible(false);
    ui->textBrowser->append(tr("��¼�ɹ���")+tr("[%1]").arg(erase)+tr("��")+tr("MAC")+tr("[%1]").arg(s.append(readCmdMac)));
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
    ui->textBrowser->setText("1����װ��nRF5x-Command-Line-Tools_8_4_0_Installer.exe\r\n\
2��C:\\Program Files\\NordicSemiconductor\\nrf5x\bin\\nrfjprog.ini ��Family��ΪNRF52\r\n\
3��ts102.hex�����뱾�ó�����ͬ��Ŀ¼��\r\n\
4����ŵ�·�����������ģ�����Ҳ����\r\n\
5���ϲ��ļ�ʱ�ѣ�nrf52832_xxaa_s132_bootloader.hex s132_nrf52_2.0.0_softdevice.hex nrf52832_xxaa_s132.hex ���ڴ�exeͬһ��Ŀ¼��\r\n\
6����ϵQQ��329410527 ");
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::black);
    ui->label->setPalette(pa);
    ui->label->setText("��¼���");
}
