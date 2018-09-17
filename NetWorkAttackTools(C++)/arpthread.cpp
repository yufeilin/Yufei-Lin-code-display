#include"arpthread.h"
#include"arp.cpp"

#include<QDebug>

//int ARP_spoof(string target_ip,string sender_ip,string target_mac)

void Arpthread::run()
{

    switch(taskid)
    {
        case 0:
            qDebug()<<"Start fool:"<<this->target_ip;
            ARP_spoof(this->target_ip.toStdString(),
                      this->gateway.toStdString(),
                      this->target_mac.toStdString());
            break;

        default:
            break;
    }
}

