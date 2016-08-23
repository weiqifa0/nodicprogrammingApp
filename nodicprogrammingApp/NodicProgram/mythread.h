#ifndef MYTHREAD
#define MYTHREAD
#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    bool stop ;
    explicit MyThread(QObject *parent = 0);
    void run();
signals:

public slots:

};
#endif // MYTHREAD

