#ifndef THREADTIAOZHANG_H
#define THREADTIAOZHANG_H


#include <QtCore>
#include<string>
using namespace std;
class Threadtiaozhang: public QThread
{
    Q_OBJECT
private:
    int number;

    std::string dev;
    std::string dst_ip;
    std::string src_ip;
    int port;
    int attack_type;

protected:
    void run();
public:
    std::string create_random_IP();

//    int construct_SYN_TCP(std::string dev,std::string dst_ip_s,int dst_port);
    int construct_SYN_TCP(string dstip,string dstmac,int dst_port);
    int construct_ACK_TCP(string dstip,string dstmac,int dst_port);
    int construct_land_TCP(string dstip,string dstmac,int dst_port);
    int construct_UDP(string dstip,string dstmac,int dst_port);
    int construct_icmp(string dev,string dstip);

    void set_dst_ip(std::string aip){dst_ip=aip;}
    void set_src_ip(std::string aip){src_ip=aip;}
    void set_dst_port(int aport){port = aport;}
    void set_dev(std::string adev){dev=adev;}
    void set_attack_type(int i){attack_type = i;}


    Threadtiaozhang(QObject *parent=0);
    ~Threadtiaozhang();
signals:

    void sendapacket(QString);

public slots:


};

#endif // THREADTIAOZHANG_H
