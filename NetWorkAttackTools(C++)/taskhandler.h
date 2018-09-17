#ifndef TASKHANDLER_H
#define TASKHANDLER_H
#include<QThread>
#include<string>
using namespace std;



class Taskhandler:public QThread{
    Q_OBJECT

private:


protected:
    void run();


public:



    int task_id;
    string targetips;

    void settaskid(int id){task_id=id;}
    void settargetips(string ips){targetips=ips;}


    static int getgatewaymac(string ip);

    static void construct_icmp(string dstip);
    static int sendicmp(string targetips);

    static int sendarp(string targetips);
    static void arp_request(char *if_name, char *dst_ip);

    static int construct_SYN_TCP(string dstip,string dstmac);
    static int send_TCP(string targetips,string dstmac);

    static int construct_UDP(string dstip,string dstmac);
    static int sendUDP(string targetips,string dstmac);

    static int construct_ACK_TCP(string dstip,string dstmac);
    static int send_ACK_TCP(string targetips,string dstmac);


    Taskhandler(QObject* parent=0);

    ~Taskhandler();

private slots:

signals:

    void finish();

};

#endif // TASKHANDLER_H
