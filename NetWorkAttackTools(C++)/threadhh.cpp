#include"threadhh.h"
#include"hhpacket.h"

#include<QDebug>


#include<stdio.h>
#include<string.h>
#include<pcap.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/if_ether.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<netinet/udp.h>
#include<netinet/tcp.h>
#include<netinet/ip_icmp.h>
#include<stdlib.h>
#include<stdint.h>
#include <stdint.h>
#include<time.h>
#include <string>
#include <net/if_arp.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <vector>

using namespace std;

#define max 1024

#define ETHER_HEADER_LEN sizeof(struct ether_header)  /* 以太网帧首部长度 */

#define ETHER_ARP_LEN sizeof(struct ether_arp)  /* 整个arp结构长度 */

#define ETHER_ARP_PACKET_LEN ETHER_HEADER_LEN + ETHER_ARP_LEN  /* 以太网 + 整个arp结构长度 */

#define IP_ADDR_LEN 4  /* IP地址长度 */


vector<packet> pkt_vct;
int pkt_num=0;




// static initialize
QString Threadhh::qsrc_mac="";
QString Threadhh::qdst_mac="";
QString Threadhh::qsrc_ip="";
QString Threadhh::qdst_ip="";
QString Threadhh::qsrc_port="";
QString Threadhh::qdst_port="";
QString Threadhh::qprotocol="";
QString Threadhh::qlen="";
QString Threadhh::qtime="";
QString Threadhh::qcont="";


static QString qtimec;






Threadhh::Threadhh(QObject *parent)
{
   packetindex=0;
}

Threadhh::~Threadhh()
{

}




void Threadhh::run()
{
//    char*argv[2];
//    argv[0]="";

//    argv[1]="100";
//    while (true) {
//        startcapture(2,argv);
//        qDebug()<<"&&&&&&&&&&&&captured1&&&&&&&&&&&&&";
//        emit captured();
//    }

    pcap_t *handle;
    pcap_if_t *alldev;
    pcap_if_t *p;
    char error[100];

    struct in_addr net_ip_addr;
    struct in_addr net_mask_addr;
    struct ether_header *ethernet;

    char *net_ip_string;
    char *net_mask_string;
    char *interface;
    u_int32_t net_ip;
    u_int32_t net_mask;

    struct pcap_pkthdr pack;
    const u_char *content;

    int i=0,num;
    if(pcap_findalldevs(&alldev,error)==-1)
    {
        printf("find all devices is error\n");
        return;
    }
    for(p=alldev;p;p=p->next)
    {
        printf("%d:%s\n",++i,p->name);
        if(p->description)
        {
            printf("%s\n",p->description);
        }
    }
    if(i==1)
        interface=p->name;
    else
    {
        printf("please input which interface you want to use\n");
//        scanf("%d",&num);
        num=1;
        //printf("num: %d\n",num);
        if(num<1||num>i)
        {
            printf("interface is unavillible\n");
            return;
        }
        for(p=alldev,i=1;i<=num;p=p->next,i++)
            interface=p->name;
    }
    /*
    if((interface=pcap_lookupdev(error))==NULL)
    {
        printf("%s\n",error);
        return 0;
    }
    */





    if((handle=pcap_open_live(interface,max,1,0,error))==NULL)
    {
        printf("%s\n",error);
        return;
    }
    if(pcap_lookupnet(interface,&net_ip,&net_mask,error)==-1)
    {
        printf("%s\n",error);
        return;
    }
    printf("Interface is:%s\n",interface);
    net_ip_addr.s_addr=net_ip;
    net_mask_addr.s_addr=net_mask;

    net_ip_string=inet_ntoa(net_ip_addr);
    printf("The ip is:%s\n",net_ip_string);

    net_mask_string=inet_ntoa(net_mask_addr);
    printf("The mask is:%s\n",net_mask_string);




    while(1){
        pcap_loop(handle,1,called,NULL);
        packetindex++;
        emit captured2(qsrc_mac,qdst_mac,qsrc_ip,qdst_ip,qsrc_port,qdst_port,
                       qprotocol,qlen,qtime,qcont);
    }



}








//Threadhh::qsrc_mac=qsrc_mac;
//Threadhh::qdst_mac=qdst_mac;
//Threadhh::qsrc_ip=qsrc_ip;
//Threadhh::qdst_ip=qdst_ip;
//Threadhh::qsrc_port=qsrc_port;
//Threadhh::qdst_port=qdst_port;
//Threadhh::qprotocol=qprotocol;
//Threadhh::qlen=qlen;
//Threadhh::qtime=qtime;


void Threadhh::called(u_char *argument,const struct pcap_pkthdr* pack,const u_char* content)
{
    int m=0,n;
    int isArp=0;
    const u_char *buf,*iphead;
    u_char *p;
    struct ether_header *ethernet;
    struct iphdr *ip;
    struct tcphdr *tcp;
    struct udphdr *udp;
    struct icmphdr *icmp;
    buf=content;

    string description;

//    printf("==================================================\n");
//    printf("The Frame is \n");
    string cont;
//    cout<<endl;
    while(m< (pack->len))
    {
        string tmp;
        //printf("%02x",buf[m]);
        stringstream contss;
        contss<<setw(2)<<setfill('0')<<hex<<(int)buf[m];
        contss>>tmp;
        m=m+1;
       if(m%16==0)
           tmp+="\n";
       else
           tmp+=":";
       //cout<<"tmp:"<<tmp<<endl;
       cont+=tmp;

    }
//    cout<<"content:"<<cont<<endl;
    int len=pack->len;

    char* time=ctime((const time_t*)&(pack->ts.tv_sec));
//    printf("Recieved at ..... %s",ctime((const time_t*)&(pack->ts.tv_sec)));
//     printf("Ethernet address length is %d\n",ETHER_HDR_LEN);

    ethernet=(struct ether_header *)content;
    p=ethernet->ether_dhost;
    n=ETHER_ADDR_LEN;
//    printf("Dest MAC is:");
    string dst_mac;
    stringstream dmss[6];
//    cout<<endl;
    int index=0;
    string dmtmp;
    do{

        if(index!=5){
            dmss[index]<<setw(2)<<setfill('0')<<hex<<(int)*p<<":";
        }
        else{
            dmss[index]<<setw(2)<<setfill('0')<<hex<<(int)*p;
        }
        dmss[index]>>dmtmp;
        dst_mac+=dmtmp;
        index++;
        p++;
    }while(--n>0);
//    cout<<"dst_mac:"<<dst_mac<<endl;

    p=ethernet->ether_shost;
    n=ETHER_ADDR_LEN;

    string src_mac;
    stringstream smss[6];
    index=0;
    string smtmp;
    do{
        if(index!=5){
            smss[index]<<setw(2)<<setfill('0')<<hex<<(int)*p<<":";
        }
        else{
            smss[index]<<setw(2)<<setfill('0')<<hex<<(int)*p;
        }
        smss[index]>>smtmp;
        src_mac+=smtmp;
        index++;
        p++;
    }while(--n>0);
//    cout<<"src_mac:"<<src_mac<<endl;

    char *src_ip,*dst_ip;
    int src_port,dst_port;
    char* protocol;
    string sport,dport;
    stringstream sp,dp;
    struct in_addr src_addr,dst_addr;
    if(ntohs(ethernet->ether_type)==ETHERTYPE_IP)
    {
        src_ip="";
        dst_ip="";
        ip=(struct iphdr*)(content+14);
//        printf("IP Version:%d\n",ip->version);
//        printf("TTL:%d\n",ip->ttl);
        src_addr.s_addr=ip->saddr;
        dst_addr.s_addr=ip->daddr;
//        printf("Source address:%s\n",inet_ntoa(src_addr));
//        printf("Destination address:%s\n",inet_ntoa(dst_addr));

        src_ip=inet_ntoa(src_addr);
        dst_ip=inet_ntoa(dst_addr);

        switch(ip->protocol)
        {
            case 1:
                icmp=(struct icmphdr*)(content+14+20);         
                switch(icmp->type)
                {
                    case 8:
                        protocol="ICMP";
                        description="ICMP ECHO request";
                        break;
                    case 0:
                        protocol="ICMP";
                        description="ICMP ECHO reply";
                        break;
                    case 3:
                        protocol="ICMP";
                        if(icmp->code==3)
                            description="ICMP DST unreachable.";
                        break;
                    default:
                        break;
                }
            break;
            case 6:
                protocol="TCP";
                description="";
                tcp=(struct tcphdr*)(content+14+20);

                if(tcp->ack==1 && tcp->syn==1){
                    description+="ACK|SYN ";
                }

                src_port=ntohs(tcp->source);
                dst_port=ntohs(tcp->dest);
                description+="source port:";
//                tcp->
                sp<<src_port;
                sp>>sport;
                description+=sport;
                description+=" destination port:";
                dp<<dst_port;
                dp>>dport;
                description+=dport;

                //int seq=ntohl(tcp->ack_seq);

//                printf("Sequence Number:%u\n",ntohl(tcp->ack_seq));
                break;
            case 17:
                protocol="UDP";
                udp=(struct udphdr*)(content+14+20);
                src_port=ntohs(udp->source);
                dst_port=ntohs(udp->dest);
                description+="source port:";

                sp<<src_port;
                sp>>sport;
                description+=sport;
                description+=" destination port:";
                dp<<dst_port;
                dp>>dport;
                description+=dport;
                break;

            default:
                break;
        }
    }


    else if(ntohs (ethernet->ether_type) == ETHERTYPE_ARP)
    {
        isArp=1;
        src_ip="";
        dst_ip="";
        protocol="ARP";
//        printf("This is ARP packet.\n");
        iphead=buf+14;
        struct ether_arp *arp_packet;
        arp_packet = (struct ether_arp *)(content + ETHER_HEADER_LEN);
        description="";
        stringstream dsp;
        string tmp;
        stringstream ss_src_ip[4];
        stringstream ss_src_mac[6];
        stringstream ss_dst_ip[4];
        stringstream ss_dst_mac[6];


        if (ntohs(arp_packet->arp_op) == 1)
        {
           // printf("==========================arp request======================\n");
            description+="REQ ";
            for (int i = 0; i < IP_ADDR_LEN; i++){
                dsp<<(arp_packet->arp_spa[i]);

                ss_src_ip[i]<<int(arp_packet->arp_spa[i]);
                ss_src_ip[i]>>tmp;
                description+=tmp;
                if(i<=2){
                    description+=".";
                }
            }
            description+=" [";
            for (int i = 0; i < ETH_ALEN; i++){
                ss_src_mac[i]<<setw(2)<<setfill('0')<<hex<<int(arp_packet->arp_sha[i]);
                ss_src_mac[i]>>tmp;
                description+=tmp;
                if(i<=4){
                    description+=":";
                }
            }
            description+="]saying who has ";
            for (int i = 0; i < IP_ADDR_LEN; i++){
                ss_dst_ip[i]<<int(arp_packet->arp_tpa[i]);
                ss_dst_ip[i]>>tmp;
                description+=tmp;
                if(i<=2){
                    description+=".";
                }
            }
//            printf("\ntarget mac");
//            description+=" ";
//            for (int i = 0; i < ETH_ALEN; i++){
//                printf(":%02x", arp_packet->arp_tha[i]);

//                ss_dst_mac[i]<<setw(2)<<setfill('0')<<hex<<int(arp_packet->arp_tha[i]);
//                ss_dst_mac[i]>>tmp;
//                description+=tmp;
//                if(i<=4){
//                    description+=":";
//                }
//            }
//            printf("\n");
        }

        if (ntohs(arp_packet->arp_op) == 2)
        {
           // printf("==========================arp reply======================\n");
            description+="REPLY ";
            for (int i = 0; i < IP_ADDR_LEN; i++){
                dsp<<(arp_packet->arp_spa[i]);

                ss_src_ip[i]<<int(arp_packet->arp_spa[i]);
                ss_src_ip[i]>>tmp;
                description+=tmp;
                if(i<=2){
                    description+=".";
                }
            }
            description+=" is at ";
            for (int i = 0; i < ETH_ALEN; i++){
                ss_src_mac[i]<<setw(2)<<setfill('0')<<hex<<int(arp_packet->arp_sha[i]);
                ss_src_mac[i]>>tmp;
                description+=tmp;
                if(i<=4){
                    description+=":";
                }
            }
            description+=" tell to: ";
            for (int i = 0; i < IP_ADDR_LEN; i++){
                ss_dst_ip[i]<<int(arp_packet->arp_tpa[i]);
                ss_dst_ip[i]>>tmp;
                description+=tmp;
                if(i<=2){
                    description+=".";
                }
            }
            description+="[";
            for (int i = 0; i < ETH_ALEN; i++){
                ss_dst_mac[i]<<setw(2)<<setfill('0')<<hex<<int(arp_packet->arp_tha[i]);
                ss_dst_mac[i]>>tmp;
                description+=tmp;
                if(i<=4){
                    description+=":";
                }
            }
            description+="]";
        }
    }






     char* des=(char*)description.data();
     char* dstmac=(char*)dst_mac.data();
     char* srcmac=(char*)src_mac.data();

     QString qsrc_mac=QString(QLatin1String(srcmac));
     QString qdst_mac=QString(QLatin1String(dstmac));
     QString qsrc_ip;
     QString qdst_ip;
     if(isArp==0){
        qsrc_ip=QString(QLatin1String(inet_ntoa(src_addr)));
        qdst_ip=QString(QLatin1String(inet_ntoa(dst_addr)));
     }
     isArp=0;
     QString qsrc_port=QString(QLatin1String(des));

     QString qdst_port=QString::number(dst_port);
     QString qprotocol=QString(QLatin1String(protocol));
     QString qlen=QString::number(len);
     QString qtime=QString(QLatin1String(time));
     QString qcont =  QString::fromStdString(cont);


     src_ip="";
     dst_ip="";


     Threadhh::qsrc_mac=qsrc_mac;
     Threadhh::qdst_mac=qdst_mac;

     Threadhh::qsrc_ip=qsrc_ip;

     Threadhh::qdst_ip=qdst_ip;

     Threadhh::qsrc_port=qsrc_port;

     Threadhh::qdst_port=qdst_port;

     Threadhh::qprotocol=qprotocol;

     Threadhh::qlen=qlen;
     Threadhh::qtime=qtime;

     Threadhh::qcont=qcont;
}
