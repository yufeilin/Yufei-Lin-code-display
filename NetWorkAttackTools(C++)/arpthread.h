#ifndef ARPTHREAD_H
#define ARPTHREAD_H
#include<QString>
#include<QThread>


class Arpthread:public QThread
{

public:
    QString target_ip;
    QString target_mac;
    QString gateway;


    int taskid;
    void setTaskid(int x)
    {
        taskid=x;
    }

    void run();

    Arpthread(){}
    ~Arpthread(){}

};

#endif // ARPTHREAD_H
