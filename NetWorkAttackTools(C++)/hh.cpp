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

using namespace std;

#define max 1024

#define ETHER_HEADER_LEN sizeof(struct ether_header)  /* 以太网帧首部长度 */

#define ETHER_ARP_LEN sizeof(struct ether_arp)  /* 整个arp结构长度 */

#define ETHER_ARP_PACKET_LEN ETHER_HEADER_LEN + ETHER_ARP_LEN  /* 以太网 + 整个arp结构长度 */

#define IP_ADDR_LEN 4  /* IP地址长度 */

/*
typedef u_int32_t int_addr_t;
struct in_addr
{
    int_addr_t s_addr;
};*/

class packet{
public:
    char* src_mac,*dst_mac;
    char* src_ip,*dst_ip;
    int src_port,dst_port;
    char* description;
    char* protocol;
    int length;
    char* time;

    packet(char* srcmac,char* dstmac,char* srcip,char* dstip,int srcport,
           int dstport,char* desc,char* protl,int len,char* ctime){
        src_mac=srcmac;
        dst_mac=dstmac;
        src_ip=srcip;
        dst_ip=dstip;
        src_port=srcport;
        dst_port=dstport;
        description=desc;
        protocol=protl;
        length=len;
        time=ctime;
    }
    char* getsrcmac(){
	return src_mac;
    }
    char* getdstmac(){
	return dst_mac;
    }
    char* getsrcip(){
	return src_ip;
    }
    char* getdstip(){
	return dst_ip;
    }
    char* getdescp(){
	return description;
    }
    char* getprotl(){
	return protocol;
    }
    int getlength(){
	return length;
    }
    char* gettime(){
	return time;
    }
};





void call(u_char *argument,const struct pcap_pkthdr* pack,const u_char* content)
{
    int m=0,n;
    const u_char *buf,*iphead;
    u_char *p;
    struct ether_header *ethernet;
    struct iphdr *ip;
    struct tcphdr *tcp;
    struct udphdr *udp;
    struct icmphdr *icmp;
    buf=content;

    string description;

    printf("==================================================\n");
    printf("The Frame is \n");
    while(m< (pack->len))
    {
        printf("%02x",buf[m]);
        m=m+1;
       if(m%16==0)
            printf("\n");
       else
            printf(":");
    }

    int len=pack->len;

    char* time=ctime((const time_t*)&(pack->ts.tv_sec));
    printf("Recieved at ..... %s",ctime((const time_t*)&(pack->ts.tv_sec)));
     printf("Ethernet address length is %d\n",ETHER_HDR_LEN);

    ethernet=(struct ether_header *)content;
    p=ethernet->ether_dhost;
    n=ETHER_ADDR_LEN;
    printf("Dest MAC is:");
    string dst_mac;
    do{
        printf("%02x:",*p++);
	//string tmp=(char*)*p++;
	//dst_mac+=tmp;
        //strcat(dst_mac,(char)*p++);
    }while(--n>0);
    //cout<<dst_mac<<endl;
    // printf("%s\n",dst_mac);
    printf("\n");
    p=ethernet->ether_shost;
    char* src_mac;
    n=ETHER_ADDR_LEN;
    printf("Source MAC is:");
    do{
        printf("%02x:",*p++);
        //strcat(src_mac,(char*)p++);
    }while(--n>0);

    printf("\n");


    char *src_ip,*dst_ip;
    int src_port,dst_port;
    char* protocol;
    string sport,dport;
    stringstream sp,dp;
    if(ntohs(ethernet->ether_type)==ETHERTYPE_IP)
    {
 	printf("It's a IP packet\n");
        ip=(struct iphdr*)(content+14);
      	printf("IP Version:%d\n",ip->version);
        printf("TTL:%d\n",ip->ttl);

	struct in_addr src_addr,dst_addr;
        src_addr.s_addr=ip->saddr;
        dst_addr.s_addr=ip->daddr;

        printf("Source address:%s\n",inet_ntoa(src_addr));
        printf("Destination address:%s\n",inet_ntoa(dst_addr));

        src_ip=inet_ntoa(src_addr);
        dst_ip=inet_ntoa(dst_addr);

        printf("Protocol:%d\n",ip->protocol);
        switch(ip->protocol)
        {
            case 1:
            printf("The Transport Layer Protocol is ICMP\n");
                icmp=(struct icmphdr*)(content+14+20);
                printf("ICMP Type:%d\n", icmp->type);

                switch(icmp->type)
                {
                    case 8:
                        protocol="ICMP";
			description="ICMP ECHO request";
                        printf("ICMP Echo Request Protocol\n");
                        break;
                    case 0:
                         protocol="ICMP";
			 description="ICMP ECHO reply";
                         printf("ICMP Echo Reply Protocol\n");
                        break;
                    default:
                        break;
            }
            break;
            case 6:
                protocol="TCP";
                description="";

                printf("The Transport Layer Protocol is TCP\n");
                tcp=(struct tcphdr*)(content+14+20);
                src_port=ntohs(tcp->source);
                dst_port=ntohs(tcp->dest);

		//sprintf(sport,"%d",src_port);
               // sprintf(dport,"%d",dst_port);
                description+="source port:";

		sp<<src_port;
		sp>>sport;
		description+=sport;
		description+=" destination port:";
		dp<<dst_port;
		dp>>dport;
		description+=dport;

                //int seq=ntohl(tcp->ack_seq);

                printf("Source Port:%d\n",ntohs(tcp->source));
                printf("Destination Port:%d\n",ntohs(tcp->dest));
                printf("Sequence Number:%u\n",ntohl(tcp->ack_seq));
                break;
            case 17:
                printf("The Transport Layer Protocol is UDP\n");
                protocol="UDP";
                udp=(struct udphdr*)(content+14+20);
                printf("Source port:%d\n",ntohs(udp->source));
                printf("Destination port:%d\n",ntohs(udp->dest));
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
/*      if(*iphead==0x45)
        {
            printf("Source ip :%d.%d.%d.%d\n",iphead[12],iphead[13],iphead[14],iphead[15]);
            printf("Dest ip :%d.%d.%d.%d\n",iphead[16],iphead[17],iphead[18],iphead[19]);

        }*/
//      tcp= (struct tcp_header*)(iphead);
//      source_port = ntohs(tcp->tcp_source_port);
//      dest_port = ntohs(tcp->tcp_destination_port);

    }
    else if(ntohs (ethernet->ether_type) == ETHERTYPE_ARP)
    {
        protocol="ARP";
        printf("This is ARP packet.\n");
        iphead=buf+14;
        struct ether_arp *arp_packet;
        arp_packet = (struct ether_arp *)(content + ETHER_HEADER_LEN);
        description="from ip:";
	stringstream dsp;
	string tmp;
	stringstream ss_src_ip[4];
	stringstream ss_src_mac[6];
	stringstream ss_dst_ip[4];
	stringstream ss_dst_mac[6];

	
	
      // if (ntohs(arp_packet->arp_op) == 2)
      // {
           // printf("==========================arp reply======================\n");
            printf("from ip:");
            for (int i = 0; i < IP_ADDR_LEN; i++){
                dsp<<(arp_packet->arp_spa[i]);
		
                ss_src_ip[i]<<int(arp_packet->arp_spa[i]);
                ss_src_ip[i]>>tmp;
                description+=tmp;
                if(i<=2){
                    description+=".";
                }
               // printf(".%u", arp_packet->arp_spa[i]);
            }
            printf("\nfrom mac");
	    description+=" from mac:";
           // strcat(description," from mac:");
            for (int i = 0; i < ETH_ALEN; i++){
                printf(":%02x", arp_packet->arp_sha[i]);

                ss_src_mac[i]<<setw(2)<<setfill('0')<<hex<<int(arp_packet->arp_sha[i]);
                ss_src_mac[i]>>tmp;
                description+=tmp;
                if(i<=4){
                    description+=":";
                }
                //int tmp=arp_packet->arp_sha[i];
               // printf("tmp: %x",tmp);
            }
           // strcat(description," target ip:");
            printf("\ntarget ip:");
	    description+=" target ip:";
            for (int i = 0; i < IP_ADDR_LEN; i++){
                printf(".%u", arp_packet->arp_tpa[i]);
                
                ss_dst_ip[i]<<int(arp_packet->arp_tpa[i]);
                ss_dst_ip[i]>>tmp;
                description+=tmp;
                if(i<=2){
                    description+=".";
                }
               // strcat(description,arp_packet->arp_tpa[i]);
            }
          //  strcat(description," target mac:");
            printf("\ntarget mac");
	    description+=" target mac:";
            for (int i = 0; i < ETH_ALEN; i++){
              //  strcat(description,arp_packet->arp_tha[i]);
                printf(":%02x", arp_packet->arp_tha[i]);
                
                ss_dst_mac[i]<<setw(2)<<setfill('0')<<hex<<int(arp_packet->arp_tha[i]);
                ss_dst_mac[i]>>tmp;
                description+=tmp;
                if(i<=4){
                    description+=":";
                }
            }
            printf("\n");
       // }
        //if (*(iphead+2)==0x08)
       // {
       //     printf("Source ip:\t %d.%d.%d.%d\n",iphead[14],iphead[15],iphead[16],iphead[17]);
       //     printf("Dest ip:\t %d.%d.%d.%d\n",iphead[24],iphead[25],iphead[26],iphead[27]);
      //      printf("ARP TYPE: %d (0:request;1:respond)\n",iphead[6]);
//
      //  }
    }
     char* des=(char*)description.data();
     char* dstmac=(char*)dst_mac.data();
    packet nowpacket(src_mac,dstmac,src_ip,dst_ip,src_port,dst_port,
                      des,protocol,len,time);
     //printf("dst_ip:%s src_ip:%s dst_port:%d src_port:%d protocol:%s desciption:%s",nowpacket.dst_ip,nowpacket.src_ip,
     //      nowpacket.dst_port,nowpacket.src_port,nowpacket.protocol,nowpacket.description);
	printf("dst_ip:%s\n",nowpacket.getdstip());
	printf("desctiption:%s\n",nowpacket.getdescp());
	printf("time:%s\n",nowpacket.gettime());
}
int amain(int argc,char *argv[])
{
    if(argc!=2)
    {
        printf("%s <number>\n",argv[0]);
        return 0;
    }
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
        return 0;
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
       	scanf("%d",&num);
        //printf("num: %d\n",num);
        if(num<1||num>i)
        {
            printf("interface is unavillible\n");
            return 0;
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
        return 0;
    }
    if(pcap_lookupnet(interface,&net_ip,&net_mask,error)==-1)
    {
        printf("%s\n",error);
        return 0;
    }
    printf("Interface is:%s\n",interface);
    net_ip_addr.s_addr=net_ip;
    net_mask_addr.s_addr=net_mask;

    net_ip_string=inet_ntoa(net_ip_addr);
    printf("The ip is:%s\n",net_ip_string);

    net_mask_string=inet_ntoa(net_mask_addr);
    printf("The mask is:%s\n",net_mask_string);

    pcap_loop(handle,atoi(argv[1]),call,NULL);

    pcap_freealldevs(alldev);
    return 1;
}


