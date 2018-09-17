
#include <libnet.h>
#include <string.h>
#include <string>
//#include <stdlib>
#include <iostream>
#include <time.h>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <iomanip>
#include <net/if_arp.h>
#include <net/if.h>
#include <netpacket/packet.h>

#include "get_gateway.cpp"
#include"spywidget.h"



using namespace std;



#define ETHER_HEADER_LEN sizeof(struct ether_header)  /* 以太网帧首部长度 */

#define ETHER_ARP_LEN sizeof(struct ether_arp)  /* 整个arp结构长度 */

#define ETHER_ARP_PACKET_LEN ETHER_HEADER_LEN + ETHER_ARP_LEN  /* 以太网 + 整个arp结构长度 */

#define IP_ADDR_LEN 4  /* IP地址长度 */

#define THREAD_NUM 100


#define BROADCAST_ADDR {0xff, 0xff, 0xff, 0xff, 0xff, 0xff} /* ¹ã²¥µØÖ· */



libnet_t *l = NULL;/* libnet句柄 */

libnet_t *handle = NULL;


void err_exit(const char *err_msg);
//{
//    perror(err_msg);
//    exit(1);
//}

struct ether_arp *fill_arp_packet(const unsigned char *src_mac_addr, const char *src_ip, const char *dst_ip)
;
//{
//    struct ether_arp *arp_packet;
//    struct in_addr src_in_addr, dst_in_addr;
//    unsigned char dst_mac_addr[ETH_ALEN] = BROADCAST_ADDR;

//    /* IPµØÖ·×ª»» */
//    inet_pton(AF_INET, src_ip, &src_in_addr);
//    inet_pton(AF_INET, dst_ip, &dst_in_addr);

//    /* Õûžöarp°ü */
//    arp_packet = (struct ether_arp *)malloc(ETHER_ARP_LEN);
//    arp_packet->arp_hrd = htons(ARPHRD_ETHER);
//    arp_packet->arp_pro = htons(ETHERTYPE_IP);
//    arp_packet->arp_hln = ETH_ALEN;
//    arp_packet->arp_pln = IP_ADDR_LEN;
//    arp_packet->arp_op = htons(ARPOP_REQUEST);
//    memcpy(arp_packet->arp_sha, src_mac_addr, ETH_ALEN);
//    memcpy(arp_packet->arp_tha, dst_mac_addr, ETH_ALEN);
//    memcpy(arp_packet->arp_spa, &src_in_addr, IP_ADDR_LEN);
//    memcpy(arp_packet->arp_tpa, &dst_in_addr, IP_ADDR_LEN);

//    return arp_packet;
//}






#include<taskhandler.h>
#include<QDebug>




Taskhandler::Taskhandler(QObject*parent){

}


Taskhandler::~Taskhandler()
{

}



void Taskhandler::run()
{
    switch (task_id) {
    case 0:
        sendarp(targetips);
        break;

    case 1:
        sendicmp(targetips);
        break;

    case 2:
        send_TCP(targetips,spyWidget::gatewaymac);
        send_ACK_TCP(targetips,spyWidget::gatewaymac);
        break;

    case 3:
        sendUDP(targetips,spyWidget::gatewaymac);
        break;

    default:
        break;
    }


    emit finish();

}








int Taskhandler::sendarp(string targetips){
    string ipaddr[256];
    int packet_length;
    for(int i=1; i<=254; i++){
        ipaddr[i]=targetips;
        stringstream tmpss;
        string tmp;
        tmpss<<i;
        tmpss>>tmp;
        ipaddr[i]+=tmp;
        //cout<<ipaddr[i]<<endl;
        arp_request("eth0", (char*)ipaddr[i].data());
    }
}

void Taskhandler::arp_request(char *if_name, char *dst_ip)
{
    struct sockaddr_ll saddr_ll;
    struct ether_header *eth_header;
    struct ether_arp *arp_packet;
    struct ifreq ifr;
    char buf[ETHER_ARP_PACKET_LEN];
    unsigned char src_mac_addr[ETH_ALEN];
    unsigned char dst_mac_addr[ETH_ALEN] = BROADCAST_ADDR;
    char *src_ip;
    int sock_raw_fd, ret_len, i;

    if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
        err_exit("socket()");

    bzero(&saddr_ll, sizeof(struct sockaddr_ll));
    bzero(&ifr, sizeof(struct ifreq));
    /* Íø¿šœÓ¿ÚÃû */
    memcpy(ifr.ifr_name, if_name, strlen(if_name));

    /* »ñÈ¡Íø¿šœÓ¿ÚË÷Òý */
    if (ioctl(sock_raw_fd, SIOCGIFINDEX, &ifr) == -1)
        err_exit("ioctl() get ifindex");
    saddr_ll.sll_ifindex = ifr.ifr_ifindex;
    saddr_ll.sll_family = PF_PACKET;

    /* »ñÈ¡Íø¿šœÓ¿ÚIP */
    if (ioctl(sock_raw_fd, SIOCGIFADDR, &ifr) == -1)
        err_exit("ioctl() get ip");
    src_ip = inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr);
//    printf("local ip:%s\n", src_ip);

    /* »ñÈ¡Íø¿šœÓ¿ÚMACµØÖ· */
    if (ioctl(sock_raw_fd, SIOCGIFHWADDR, &ifr))
        err_exit("ioctl() get mac");
    memcpy(src_mac_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
//    printf("local mac");
//    for (i = 0; i < ETH_ALEN; i++)
//        printf(":%02x", src_mac_addr[i]);
//    printf("\n");

    bzero(buf, ETHER_ARP_PACKET_LEN);
    /* Ìî³äÒÔÌ«Ê×²¿ */
    eth_header = (struct ether_header *)buf;
    memcpy(eth_header->ether_shost, src_mac_addr, ETH_ALEN);
    memcpy(eth_header->ether_dhost, dst_mac_addr, ETH_ALEN);
    eth_header->ether_type = htons(ETHERTYPE_ARP);
    /* arp°ü */
    arp_packet = fill_arp_packet(src_mac_addr, src_ip, dst_ip);
    memcpy(buf + ETHER_HEADER_LEN, arp_packet, ETHER_ARP_LEN);

    /* ·¢ËÍÇëÇó */
    ret_len = sendto(sock_raw_fd, buf, ETHER_ARP_PACKET_LEN, 0, (struct sockaddr *)&saddr_ll, sizeof(struct sockaddr_ll));
//    if ( ret_len > 0)
//        printf("sendto() ok!!!\n");

    close(sock_raw_fd);
}


int Taskhandler::getgatewaymac(string ip)
{
    int i=5;
    while(i--)
    {
        arp_request("eth0", (char*)ip.data());
//        printf("Taskhandler::getgatewaymac sending arp %s\n",(char*)ip.data());
    }

    return 0;
}

void Taskhandler::construct_icmp(string dstip){

    libnet_ptag_t protocol_tag;/* 协议标记 */
    char *payload_liu_wen_tao = NULL; /* 负载 */
    u_short payload_length = 0; /* 负载长度 */
    char device[] = "eth0";/*网络设备接口*/

    u_long source_ip = 0; /* 源IP地址 */
    u_long destination_ip = 0; /* 目的IP地址 */
    char errbuf[LIBNET_ERRBUF_SIZE]; /* 错误信息 */
    int packet_length; /* 发送的数据包的长度 */
    l = libnet_init( /* 初始化libnet */
    LIBNET_RAW4, /* libnet类型，为原始套接字IPv4类型 */
    device,  /* 网络设备接口 */errbuf /* 错误信息 */
    );
    source_ip = libnet_name2addr4(l, "192.168.153.136" , LIBNET_RESOLVE);
    /* 把源IP地址字符串形式转化为网络字节顺序的数据 */
    destination_ip = libnet_name2addr4(l, (char*)dstip.data(), LIBNET_RESOLVE);
    /* 把目的IP地址字符串形式转化为网络字节顺序的数据 */
    protocol_tag = libnet_build_icmpv4_echo( /* 构造ICMP回显数据包 */
    ICMP_ECHO, /* 类型，此时为回显请求 */
    0,/* 代码，应该为0 */
    0, /* 校验和，为0，表示由libnet句柄自动计算 */
    123,  /* 标识符，赋值为123，自己任意填写数值 */
    456, /* 序列号，赋值为245，自己任意填写数值 */
    NULL,  /* 负载，赋值为空 */
    0, /* 负载的长度，赋值为0 */
    l, /* libnet句柄，应该是由libnet_init()函数得到的 */
    0/* 协议块标记，赋值为0，表示构造一个新的协议块 */
    );
    protocol_tag = libnet_build_ipv4(/* 构造IP协议块 */
    LIBNET_IPV4_H + LIBNET_ICMPV4_ECHO_H + payload_length,/* IP协议块的长度 */
    0, /* 服务质量，这里赋值为0 */
    10,  /* 标识符，这里赋值为10 */
    0, /* 偏移，这里赋值为0 */
    20,/* 生存时间，这里赋值为20 */
    IPPROTO_ICMP,/* 上层协议类型，这里是ICMP协议 */
    0, /* 校验和，这里为0表示由libnet计算校验和 */
    source_ip, /* 源IP地址 */
    destination_ip,/* 目的IP地址 */
    (const uint8_t*)payload_liu_wen_tao, /* 负载 */
    payload_length, /* 负载的长度 */
    l,/* libnet句柄 */
    0/* 协议块标记，为0表示构造一个新的IP协议块 */
  );
}

int Taskhandler::sendicmp(string targetips)
{

    string ipaddr[256];
    int packet_length;
    for(int i=1; i<=254; i++){
    ipaddr[i]=targetips;
    stringstream tmpss;
    string tmp;
    tmpss<<i;
    tmpss>>tmp;
    ipaddr[i]+=tmp;
//    cout<<ipaddr[i]
//
//  <<endl;
    construct_icmp(ipaddr[i]);
        packet_length = libnet_write(l); /* 发送由libnet句柄l表示的数据包 */



//        printf("the length of the ICMP packet is %d\n", packet_length);
    libnet_destroy(l); /* 销毁libnet */
        /* 输出发送的数据包信息 */
    }
}

int Taskhandler::construct_SYN_TCP(string dstip,string dstmac)
{

    char* dst_ip_s;
    char* src_ip_s;
//    dstmac="";
    unsigned char src_mac_addr[ETH_ALEN];
    int sock_raw_fd, i;

    if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
        err_exit("socket()");

//    bzero(&saddr_ll, sizeof(struct sockaddr_ll));
//    bzero(&ifr, sizeof(struct ifreq));


    srand((unsigned)time(0));
//    string src_ip_s = "192.168.153.136";

//    libnet_t *handle /* LibnetŸä±ú */
    int packet_size; /* ¹¹ÔìµÄÊýŸÝ°üŽóÐ¡ */

    char device[] = "eth0"; /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */

    char* dst_ip_str=(char*)dstip.data();

//    printf("dstIP:%s\n",dst_ip_str);

    struct ifreq ifr;
    bzero(&ifr, sizeof(struct ifreq));
    char* if_name="eth0";
    memcpy(ifr.ifr_name, if_name, strlen(if_name));
    if (ioctl(sock_raw_fd, SIOCGIFADDR, &ifr) == -1)
        err_exit("ioctl() get ip");
    src_ip_s = inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr);

    if (ioctl(sock_raw_fd, SIOCGIFHWADDR, &ifr))
        err_exit("ioctl() get mac");
    memcpy(src_mac_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
//    printf("local mac");
//    for (i = 0; i < ETH_ALEN; i++)
//        printf(":%02x", src_mac_addr[i]);
//    printf("\n");


    u_char src_mac[6];
    for(int i=0;i<6;i++){
        src_mac[i]=src_mac_addr[i];
    }



   // u_char src_macss[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ÔŽMAC */
    u_char dst_mac[6]; /* Ä¿µÄMAC */
    string tmp[6];


//    printf("before\n");
    for(int i=0;i<6;i++)
    {
        tmp[i].resize(2);
        tmp[i][0]=dstmac[i*3];
        tmp[i][1]=dstmac[i*3+1];
//        printf("tmp:%s\n",tmp[i].data());
    }
    int macvalues[6];
    for(int i=0;i<6;i++)
    {
        macvalues[i]=0;
        for(int j=0;j<2;j++)
        {
            int tmpa=0;
            switch (tmp[i][j]) {
            case '0':
                tmpa=0;
                break;
            case '1':
                tmpa=1;
                break;
            case '2':
                tmpa=2;
                break;
            case '3':
                tmpa=3;
                break;
            case '4':
                tmpa=4;
                break;
            case '5':
                tmpa=5;
                break;
            case '6':
                tmpa=6;
                break;
            case '7':
                tmpa=7;
                break;
            case '8':
                tmpa=8;
                break;
            case '9':
                tmpa=9;
                break;
            case 'a':
                tmpa=10;
                break;
            case 'b':
                tmpa=11;
                break;
            case 'c':
                tmpa=12;
                break;
            case 'd':
                tmpa=13;
                break;
            case 'e':
                tmpa=14;
                break;
            case 'f':
                tmpa=15;
                break;
            default:
                break;
            }

            if(j==0)
                macvalues[i]+=16*tmpa;
            else
                macvalues[i]+=tmpa;
        }
    }
//    printf("after\n");

//    for(int i=0;i<6;i++){
//        printf("tmpp:%s",tmp[i].data());
//        printf("------value:%d\n",macvalues[i]);
//    }

    stringstream tmpss[6];
    for(int i=0; i<6; i++){
//        printf("tmp:%s\n",tmp[i].data());
//        tmpss[i]<<tmp[i];
//        int tmpnum;
//        tmpss[i]>>tmpnum;
        dst_mac[i]=macvalues[i];
//        printf("tmpn:%d::",dst_mac[i]);
    }
//    printf("\n");

//    u_long dst_ip, src_ip; /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ž÷²ãbuildº¯Êý·µ»ØÖµ */
    u_short proto = IPPROTO_TCP; /* Ž«Êä²ãÐ­Òé */
    u_char payload[255] = {0}; /* ³ÐÔØÊýŸÝµÄÊý×é£¬³õÖµÎª¿Õ */
    u_long payload_s = 0; /* ³ÐÔØÊýŸÝµÄ³€¶È£¬³õÖµÎª0 */
    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    u_long dst_ip = libnet_name2addr4(handle, dst_ip_str, LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    u_long src_ip = libnet_name2addr4(handle, src_ip_s, LIBNET_RESOLVE);
    /* ³õÊŒ»¯Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, device, error)) == NULL )
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* ¹¹ÔìžºÔØµÄÄÚÈÝ */
    payload_s = strlen((char*)payload); /* ŒÆËãžºÔØÄÚÈÝµÄ³€¶È */

#if 0
    /* ¹¹œšTCPµÄÑ¡Ïî,Íš³£ÔÚµÚÒ»žöTCPÍšÐÅ±šÎÄÖÐÉèÖÃMSS */
    tcp_op_tag = libnet_build_tcp_options(
                payload,
                payload_s,
                handle,
                0
    );
    if (tcp_op_tag == -1) {
        printf("build_tcp_options failure\n");
        return (-2);
    };
#endif

    int rand_src_port = rand()%65535;
    int dst_port=80;
    tcp_tag = libnet_build_tcp(
                (uint16_t)rand_src_port, /* ÔŽ¶Ë¿Ú */
                (uint16_t)dst_port,      /* Ä¿µÄ¶Ë¿Ú */
                8888,                    /* ÐòÁÐºÅ */
                8889,                    /* È·ÈÏºÅ */
                TH_SYN,                   /* Control flags */
                14600,                    /* Ž°¿Ú³ßŽç */
                0,                        /* Ð£ÑéºÍ,0Îª×Ô¶¯ŒÆËã */
                0,                        /* œôŒ±ÖžÕë */
                LIBNET_TCP_H + payload_s, /* ³€¶È */
                payload,                    /* žºÔØÄÚÈÝ */
                payload_s,                /* žºÔØÄÚÈÝ³€¶È */
                handle,                    /* libnetŸä±ú */
                0                        /* ÐÂœš°ü */
    );

    if (tcp_tag == -1)
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* ¹¹ÔìIPÐ­Òé¿é£¬·µ»ØÖµÊÇÐÂÉú³ÉµÄIPÐ­Òé¿ìµÄÒ»žö±êŒÇ */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IPÐ­Òé¿éµÄ×Ü³€,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,Ëæ»ú²úÉú0~65535 */
        0, /* frag Æ¬Æ«ÒÆ */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,Ëæ»ú²úÉú0~255 */
        proto, /* ÉÏ²ãÐ­Òé */
        0, /* Ð£ÑéºÍ£¬ŽËÊ±Îª0£¬±íÊŸÓÉLibnet×Ô¶¯ŒÆËã */
        src_ip, /* ÔŽIPµØÖ·,ÍøÂçÐò */
        dst_ip, /* Ä¿±êIPµØÖ·,ÍøÂçÐò */
        NULL, /* žºÔØÄÚÈÝ»òÎªNULL */
        0, /* žºÔØÄÚÈÝµÄŽóÐ¡*/
        handle, /* LibnetŸä±ú */
        0 /* Ð­Òé¿é±êŒÇ¿ÉÐÞžÄ»òŽŽœš,0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* ¹¹ÔìÒ»žöÒÔÌ«ÍøÐ­Òé¿é,Ö»ÄÜÓÃÓÚLIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* ÒÔÌ«ÍøÄ¿µÄµØÖ· */
        src_mac, /* ÒÔÌ«ÍøÔŽµØÖ· */
        ETHERTYPE_IP, /* ÒÔÌ«ÍøÉÏ²ãÐ­ÒéÀàÐÍ£¬ŽËÊ±ÎªIPÀàÐÍ */
        NULL, /* žºÔØ£¬ÕâÀïÎª¿Õ */
        0, /* žºÔØŽóÐ¡ */
        handle, /* LibnetŸä±ú */
        0 /* Ð­Òé¿é±êŒÇ£¬0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ */
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    //before sending
}


int Taskhandler::send_TCP(string targetips,string dstmac){

//    printf("dstmac %s",dstmac.data());
    string ipaddr[256];
    int packet_length;
    for(int i=1; i<=254; i++){
        ipaddr[i]=targetips;
        stringstream tmpss;
        string tmp;
        tmpss<<i;
        tmpss>>tmp;
        ipaddr[i]+=tmp;
//        cout<<ipaddr[i]<<endl;

        construct_SYN_TCP(ipaddr[i],dstmac);
        packet_length = libnet_write(handle); /* 发送由libnet句柄l表示的数据包 */
//        printf("the length of the ICMP packet is %d\n", packet_length);
//        printf("*&****&^^&(*(*\n");
        libnet_destroy(handle); /* 销毁libnet */
        /* 输出发送的数据包信息 */
    }
//     printf("dstmac %s",dstmac.data());
}

int Taskhandler::construct_ACK_TCP(string dstip,string dstmac)
{

    char* dst_ip_s;
    char* src_ip_s;
//    dstmac="";
    unsigned char src_mac_addr[ETH_ALEN];
    int sock_raw_fd, i;

    if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
        err_exit("socket()");

//    bzero(&saddr_ll, sizeof(struct sockaddr_ll));
//    bzero(&ifr, sizeof(struct ifreq));


    srand((unsigned)time(0));
//    string src_ip_s = "192.168.153.136";

//    libnet_t *handle /* LibnetŸä±ú */
    int packet_size; /* ¹¹ÔìµÄÊýŸÝ°üŽóÐ¡ */

    char device[] = "eth0"; /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */

    char* dst_ip_str=(char*)dstip.data();

//    printf("dstIP:%s\n",dst_ip_str);

    struct ifreq ifr;
    bzero(&ifr, sizeof(struct ifreq));
    char* if_name="eth0";
    memcpy(ifr.ifr_name, if_name, strlen(if_name));
    if (ioctl(sock_raw_fd, SIOCGIFADDR, &ifr) == -1)
        err_exit("ioctl() get ip");
    src_ip_s = inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr);

    if (ioctl(sock_raw_fd, SIOCGIFHWADDR, &ifr))
        err_exit("ioctl() get mac");
    memcpy(src_mac_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
//    printf("local mac");
//    for (i = 0; i < ETH_ALEN; i++)
//        printf(":%02x", src_mac_addr[i]);
//    printf("\n");


    u_char src_mac[6];
    for(int i=0;i<6;i++){
        src_mac[i]=src_mac_addr[i];
    }



   // u_char src_macss[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ÔŽMAC */
    u_char dst_mac[6]; /* Ä¿µÄMAC */
    string tmp[6];


//    printf("before\n");
    for(int i=0;i<6;i++)
    {
        tmp[i].resize(2);
        tmp[i][0]=dstmac[i*3];
        tmp[i][1]=dstmac[i*3+1];
//        printf("tmp:%s\n",tmp[i].data());
    }
    int macvalues[6];
    for(int i=0;i<6;i++)
    {
        macvalues[i]=0;
        for(int j=0;j<2;j++)
        {
            int tmpa=0;
            switch (tmp[i][j]) {
            case '0':
                tmpa=0;
                break;
            case '1':
                tmpa=1;
                break;
            case '2':
                tmpa=2;
                break;
            case '3':
                tmpa=3;
                break;
            case '4':
                tmpa=4;
                break;
            case '5':
                tmpa=5;
                break;
            case '6':
                tmpa=6;
                break;
            case '7':
                tmpa=7;
                break;
            case '8':
                tmpa=8;
                break;
            case '9':
                tmpa=9;
                break;
            case 'a':
                tmpa=10;
                break;
            case 'b':
                tmpa=11;
                break;
            case 'c':
                tmpa=12;
                break;
            case 'd':
                tmpa=13;
                break;
            case 'e':
                tmpa=14;
                break;
            case 'f':
                tmpa=15;
                break;
            default:
                break;
            }

            if(j==0)
                macvalues[i]+=16*tmpa;
            else
                macvalues[i]+=tmpa;
        }
    }
//    printf("after\n");

    for(int i=0;i<6;i++){
        printf("tmpp:%s",tmp[i].data());
        printf("------value:%d\n",macvalues[i]);
    }

    stringstream tmpss[6];
    for(int i=0; i<6; i++){
//        printf("tmp:%s\n",tmp[i].data());
//        tmpss[i]<<tmp[i];
//        int tmpnum;
//        tmpss[i]>>tmpnum;
        dst_mac[i]=macvalues[i];
        printf("tmpn:%d::",dst_mac[i]);
    }
//    printf("\n");

//    u_long dst_ip, src_ip; /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ž÷²ãbuildº¯Êý·µ»ØÖµ */
    u_short proto = IPPROTO_TCP; /* Ž«Êä²ãÐ­Òé */
    u_char payload[255] = {0}; /* ³ÐÔØÊýŸÝµÄÊý×é£¬³õÖµÎª¿Õ */
    u_long payload_s = 0; /* ³ÐÔØÊýŸÝµÄ³€¶È£¬³õÖµÎª0 */
    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    u_long dst_ip = libnet_name2addr4(handle, dst_ip_str, LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    u_long src_ip = libnet_name2addr4(handle, src_ip_s, LIBNET_RESOLVE);
    /* ³õÊŒ»¯Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, device, error)) == NULL )
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* ¹¹ÔìžºÔØµÄÄÚÈÝ */
    payload_s = strlen((char*)payload); /* ŒÆËãžºÔØÄÚÈÝµÄ³€¶È */

#if 0
    /* ¹¹œšTCPµÄÑ¡Ïî,Íš³£ÔÚµÚÒ»žöTCPÍšÐÅ±šÎÄÖÐÉèÖÃMSS */
    tcp_op_tag = libnet_build_tcp_options(
                payload,
                payload_s,
                handle,
                0
    );
    if (tcp_op_tag == -1) {
        printf("build_tcp_options failure\n");
        return (-2);
    };
#endif

    int rand_src_port = rand()%65535;
    int dst_port=80;
    tcp_tag = libnet_build_tcp(
                (uint16_t)rand_src_port, /* ÔŽ¶Ë¿Ú */
                (uint16_t)dst_port,      /* Ä¿µÄ¶Ë¿Ú */
                8888,                    /* ÐòÁÐºÅ */
                8889,                    /* È·ÈÏºÅ */
                TH_ACK,                   /* Control flags */
                14600,                    /* Ž°¿Ú³ßŽç */
                0,                        /* Ð£ÑéºÍ,0Îª×Ô¶¯ŒÆËã */
                0,                        /* œôŒ±ÖžÕë */
                LIBNET_TCP_H + payload_s, /* ³€¶È */
                payload,                    /* žºÔØÄÚÈÝ */
                payload_s,                /* žºÔØÄÚÈÝ³€¶È */
                handle,                    /* libnetŸä±ú */
                0                        /* ÐÂœš°ü */
    );

    if (tcp_tag == -1)
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* ¹¹ÔìIPÐ­Òé¿é£¬·µ»ØÖµÊÇÐÂÉú³ÉµÄIPÐ­Òé¿ìµÄÒ»žö±êŒÇ */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IPÐ­Òé¿éµÄ×Ü³€,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,Ëæ»ú²úÉú0~65535 */
        0, /* frag Æ¬Æ«ÒÆ */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,Ëæ»ú²úÉú0~255 */
        proto, /* ÉÏ²ãÐ­Òé */
        0, /* Ð£ÑéºÍ£¬ŽËÊ±Îª0£¬±íÊŸÓÉLibnet×Ô¶¯ŒÆËã */
        src_ip, /* ÔŽIPµØÖ·,ÍøÂçÐò */
        dst_ip, /* Ä¿±êIPµØÖ·,ÍøÂçÐò */
        NULL, /* žºÔØÄÚÈÝ»òÎªNULL */
        0, /* žºÔØÄÚÈÝµÄŽóÐ¡*/
        handle, /* LibnetŸä±ú */
        0 /* Ð­Òé¿é±êŒÇ¿ÉÐÞžÄ»òŽŽœš,0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* ¹¹ÔìÒ»žöÒÔÌ«ÍøÐ­Òé¿é,Ö»ÄÜÓÃÓÚLIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* ÒÔÌ«ÍøÄ¿µÄµØÖ· */
        src_mac, /* ÒÔÌ«ÍøÔŽµØÖ· */
        ETHERTYPE_IP, /* ÒÔÌ«ÍøÉÏ²ãÐ­ÒéÀàÐÍ£¬ŽËÊ±ÎªIPÀàÐÍ */
        NULL, /* žºÔØ£¬ÕâÀïÎª¿Õ */
        0, /* žºÔØŽóÐ¡ */
        handle, /* LibnetŸä±ú */
        0 /* Ð­Òé¿é±êŒÇ£¬0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ */
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    //before sending
}

int Taskhandler::send_ACK_TCP(string targetips,string dstmac){

//    printf("dstmac %s",dstmac.data());
    string ipaddr[256];
    int packet_length;
    for(int i=1; i<=254; i++){
        ipaddr[i]=targetips;
        stringstream tmpss;
        string tmp;
        tmpss<<i;
        tmpss>>tmp;
        ipaddr[i]+=tmp;
//        cout<<ipaddr[i]<<endl;
        construct_SYN_TCP(ipaddr[i],dstmac);
        packet_length = libnet_write(handle); /* 发送由libnet句柄l表示的数据包 */
//        printf("the length of the ICMP packet is %d\n", packet_length);
//        printf("*&****&^^&(*(*\n");
        libnet_destroy(handle); /* 销毁libnet */
        /* 输出发送的数据包信息 */
    }
//     printf("dstmac %s",dstmac.data());
}

int Taskhandler::construct_UDP(string dstip,string dstmac)
{
    char* src_ip_s;
//    dstmac="";
    unsigned char src_mac_addr[ETH_ALEN];
    int sock_raw_fd, i;

    if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
        err_exit("socket()");

    srand((unsigned)time(0));

    int packet_size; /* ¹¹ÔìµÄÊýŸÝ°üŽóÐ¡ */

    char device[] = "eth0"; /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */

    char* dst_ip_s=(char*)dstip.data();

//    printf("dstIP:%s\n",dst_ip_str);

    struct ifreq ifr;
    bzero(&ifr, sizeof(struct ifreq));
    char* if_name="eth0";
    memcpy(ifr.ifr_name, if_name, strlen(if_name));
    if (ioctl(sock_raw_fd, SIOCGIFADDR, &ifr) == -1)
        err_exit("ioctl() get ip");
    src_ip_s = inet_ntoa(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr);

    if (ioctl(sock_raw_fd, SIOCGIFHWADDR, &ifr))
        err_exit("ioctl() get mac");
    memcpy(src_mac_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
//    printf("local mac");
//    for (i = 0; i < ETH_ALEN; i++)
//        printf(":%02x", src_mac_addr[i]);
//    printf("\n");


    u_char src_mac[6];
    for(int i=0;i<6;i++){
        src_mac[i]=src_mac_addr[i];
    }

   // u_char src_macss[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ÔŽMAC */
    u_char dst_mac[6]; /* Ä¿µÄMAC */
    string tmp[6];


//    printf("before\n");
    for(int i=0;i<6;i++)
    {
        tmp[i].resize(2);
        tmp[i][0]=dstmac[i*3];
        tmp[i][1]=dstmac[i*3+1];
//        printf("tmp:%s\n",tmp[i].data());
    }
    int macvalues[6];
    for(int i=0;i<6;i++)
    {
        macvalues[i]=0;
        for(int j=0;j<2;j++)
        {
            int tmpa=0;
            switch (tmp[i][j]) {
            case '0':
                tmpa=0;
                break;
            case '1':
                tmpa=1;
                break;
            case '2':
                tmpa=2;
                break;
            case '3':
                tmpa=3;
                break;
            case '4':
                tmpa=4;
                break;
            case '5':
                tmpa=5;
                break;
            case '6':
                tmpa=6;
                break;
            case '7':
                tmpa=7;
                break;
            case '8':
                tmpa=8;
                break;
            case '9':
                tmpa=9;
                break;
            case 'a':
                tmpa=10;
                break;
            case 'b':
                tmpa=11;
                break;
            case 'c':
                tmpa=12;
                break;
            case 'd':
                tmpa=13;
                break;
            case 'e':
                tmpa=14;
                break;
            case 'f':
                tmpa=15;
                break;
            default:
                break;
            }

            if(j==0)
                macvalues[i]+=16*tmpa;
            else
                macvalues[i]+=tmpa;
        }
    }
//    printf("after\n");

    for(int i=0;i<6;i++){
        printf("tmpp:%s",tmp[i].data());
        printf("------value:%d\n",macvalues[i]);
    }

    stringstream tmpss[6];
    for(int i=0; i<6; i++){
        dst_mac[i]=macvalues[i];
//        printf("tmpn:%d::",dst_mac[i]);
    }
//    printf("\n");

//    u_long dst_ip, src_ip; /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ž÷²ãbuildº¯Êý·µ»ØÖµ */
    u_char payload[255] = {0}; /* ³ÐÔØÊýŸÝµÄÊý×é£¬³õÖµÎª¿Õ */
    u_long payload_s = 0; /* ³ÐÔØÊýŸÝµÄ³€¶È£¬³õÖµÎª0 */
    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    u_long dst_ip = libnet_name2addr4(handle, dst_ip_s, LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    u_long src_ip = libnet_name2addr4(handle, src_ip_s, LIBNET_RESOLVE);
    /* ³õÊŒ»¯Libnet */

    if ((handle = libnet_init(LIBNET_LINK, device, error)) == NULL )
    {
        printf("libnet_init failure\n");
        return (-1);
    };



    strncpy((char*)payload, "", sizeof(payload)-1); /* ¹¹ÔìžºÔØµÄÄÚÈÝ */
    payload_s = strlen((char*)payload); /* ŒÆËãžºÔØÄÚÈÝµÄ³€¶È */


    libnet_ptag_t udp_tag; /* ž÷²ãbuildº¯Êý·µ»ØÖµ */
    u_short proto = IPPROTO_UDP; /* Ž«Êä²ãÐ­Òé */

    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    dst_ip = libnet_name2addr4(handle, dst_ip_s, LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    src_ip = libnet_name2addr4(handle, src_ip_s, LIBNET_RESOLVE);

    /* ³õÊŒ»¯Libnet */

    strncpy((char*)payload, "testfwfewfwfeqwefqefqfe", sizeof(payload)-1); /* ¹¹ÔìžºÔØµÄÄÚÈÝ */
    payload_s = strlen((char*)payload); /* ŒÆËãžºÔØÄÚÈÝµÄ³€¶È */

    u_short rand_src_port = (u_short)rand()%6553;

    u_short u_dst_port = 80;




    udp_tag = libnet_build_udp(
                rand_src_port, /* ÔŽ¶Ë¿Ú */
                u_dst_port, /* Ä¿µÄ¶Ë¿Ú */
                LIBNET_UDP_H + payload_s, /* ³€¶È */
                0, /* Ð£ÑéºÍ,0Îªlibnet×Ô¶¯ŒÆËã */
                payload, /* žºÔØÄÚÈÝ */
                payload_s, /* žºÔØÄÚÈÝ³€¶È */
                handle, /* libnetŸä±ú */
                0 /* ÐÂœš°ü */
    );
    if (udp_tag == -1) {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* ¹¹ÔìIPÐ­Òé¿é£¬·µ»ØÖµÊÇÐÂÉú³ÉµÄIPÐ­Òé¿ìµÄÒ»žö±êŒÇ */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_UDP_H + payload_s, /* IPÐ­Òé¿éµÄ×Ü³€,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,Ëæ»ú²úÉú0~65535 */
        0, /* frag Æ¬Æ«ÒÆ */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,Ëæ»ú²úÉú0~255 */
        proto, /* ÉÏ²ãÐ­Òé */
        0, /* Ð£ÑéºÍ£¬ŽËÊ±Îª0£¬±íÊŸÓÉLibnet×Ô¶¯ŒÆËã */
        src_ip, /* ÔŽIPµØÖ·,ÍøÂçÐò */
        dst_ip, /* Ä¿±êIPµØÖ·,ÍøÂçÐò */
        NULL, /* žºÔØÄÚÈÝ»òÎªNULL */
        0, /* žºÔØÄÚÈÝµÄŽóÐ¡*/
        handle, /* LibnetŸä±ú */
        0 /* Ð­Òé¿é±êŒÇ¿ÉÐÞžÄ»òŽŽœš,0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ*/
    );


    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };

    /* ¹¹ÔìÒ»žöÒÔÌ«ÍøÐ­Òé¿é,Ö»ÄÜÓÃÓÚLIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* ÒÔÌ«ÍøÄ¿µÄµØÖ· */
        src_mac, /* ÒÔÌ«ÍøÔŽµØÖ· */
        ETHERTYPE_IP, /* ÒÔÌ«ÍøÉÏ²ãÐ­ÒéÀàÐÍ£¬ŽËÊ±ÎªIPÀàÐÍ */
        NULL, /* žºÔØ£¬ÕâÀïÎª¿Õ */
        0, /* žºÔØŽóÐ¡ */
        handle, /* LibnetŸä±ú */
        0 /* Ð­Òé¿é±êŒÇ£¬0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ */
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    }

    return (0);
}

int Taskhandler::sendUDP(string targetips, string dstmac){
//    printf("dstmac %s",dstmac.data());
    string ipaddr[256];
    int packet_length;
    for(int i=1; i<=254; i++){
        ipaddr[i]=targetips;
        stringstream tmpss;
        string tmp;
        tmpss<<i;
        tmpss>>tmp;
        ipaddr[i]+=tmp;
//        cout<<ipaddr[i]<<endl;

        //construct_SYN_TCP(ipaddr[i],dstmac);
        construct_UDP(ipaddr[i],dstmac);
        packet_length = libnet_write(handle); /* 发送由libnet句柄l表示的数据包 */
//        printf("the length of the ICMP packet is %d\n", packet_length);
//        printf("*&****&^^&(*(*\n");
        libnet_destroy(handle); /* 销毁libnet */
        /* 输出发送的数据包信息 */
    }
}
