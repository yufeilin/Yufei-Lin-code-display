#ifndef THREADHH_H
#define THREADHH_H


#include"hhpacket.h"
#include <QtCore>
#include<string>
using namespace std;





//packet Threadhh::packetnow=NULL;


class Threadhh: public QThread
{
    Q_OBJECT
private:
protected:
    void run();
public:




    static QString qsrc_mac;
    static QString qdst_mac;
    static QString qsrc_ip;
    static QString qdst_ip;
    static QString qsrc_port;

    static QString qdst_port;
    static QString qprotocol;
    static QString qlen;
    static QString qtime;

    static QString qcont;


    static int packetindex;

    static packet packetnow;

    static void called(u_char *argument,const struct pcap_pkthdr* pack,const u_char* content);
    int startcapture(int argc,char *argv[]);
//    void call(u_char *argument,const struct pcap_pkthdr* pack,const u_char* content);

    Threadhh(QObject *parent=0);
    ~Threadhh();

    void monitor();

signals:

    void captured();

    void getsomepacket(QString);

    void captured2(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);


public slots:


};




#endif // THREADHH_H
