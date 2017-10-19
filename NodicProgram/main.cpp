#include "mainwindow.h"
#include <QApplication>
#include "mythread.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString runPath = QCoreApplication::applicationDirPath();
    w.setWindowTitle("韦启发V1.1_漂流尾巴APPHEX烧录软件["+runPath+"]");
    w.show();
    return a.exec();
}
