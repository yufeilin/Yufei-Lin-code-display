#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <libnet.h>
#include <string>

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
#include<time.h>
#include <string>
#include <net/if_arp.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <iomanip>




//#include"tiaozhangbao.cpp"

using namespace std;
#define max 1024

#define ETHER_HEADER_LEN sizeof(struct ether_header)  /* 以太网帧首部长度 */

#define ETHER_ARP_LEN sizeof(struct ether_arp)  /* 整个arp结构长度 */

#define ETHER_ARP_PACKET_LEN ETHER_HEADER_LEN + ETHER_ARP_LEN  /* 以太网 + 整个arp结构长度 */

#define IP_ADDR_LEN 4  /* IP地址长度 */

#define THREAD_NUM 100

libnet_t *lib = NULL;/* libnet句柄 */

void construct_icmp(string dstip){

    libnet_ptag_t protocol_tag;/* 协议标记 */
    char *payload_liu_wen_tao = NULL; /* 负载 */
    u_short payload_length = 0; /* 负载长度 */
    char device[] = "eth0";/*网络设备接口*/

    u_long source_ip = 0; /* 源IP地址 */
    u_long destination_ip = 0; /* 目的IP地址 */
    char errbuf[LIBNET_ERRBUF_SIZE]; /* 错误信息 */
    int packet_length; /* 发送的数据包的长度 */
    lib = libnet_init( /* 初始化libnet */
    LIBNET_RAW4, /* libnet类型，为原始套接字IPv4类型 */
    device,  /* 网络设备接口 */errbuf /* 错误信息 */
    );
    source_ip = libnet_name2addr4(lib, "192.168.153.136" , LIBNET_RESOLVE);
    /* 把源IP地址字符串形式转化为网络字节顺序的数据 */
    destination_ip = libnet_name2addr4(lib, (char*)dstip.data(), LIBNET_RESOLVE);
    /* 把目的IP地址字符串形式转化为网络字节顺序的数据 */
    protocol_tag = libnet_build_icmpv4_echo( /* 构造ICMP回显数据包 */
    ICMP_ECHO, /* 类型，此时为回显请求 */
    0,/* 代码，应该为0 */
    0, /* 校验和，为0，表示由libnet句柄自动计算 */
    123,  /* 标识符，赋值为123，自己任意填写数值 */
    456, /* 序列号，赋值为245，自己任意填写数值 */
    NULL,  /* 负载，赋值为空 */
    0, /* 负载的长度，赋值为0 */
    lib, /* libnet句柄，应该是由libnet_init()函数得到的 */
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
    lib,/* libnet句柄 */
    0/* 协议块标记，为0表示构造一个新的IP协议块 */
  );
}



int sendicmp(string targetips)
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
    cout<<ipaddr[i]<<endl;
    construct_icmp(ipaddr[i]);
        packet_length = libnet_write(lib); /* 发送由libnet句柄l表示的数据包 */



        printf("the length of the ICMP packet is %d\n", packet_length);
    libnet_destroy(lib); /* 销毁libnet */
        /* 输出发送的数据包信息 */
    }
}

int sendarp(string targetips){
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
//        arp_request("eth0", (char*)ipaddr[i].data());
    }
}


//int construct_SYN_TCP(string dev,string dst_ip_s,int dst_port)
//{

//    srand((unsigned)time(0));


//    string src_ip_s = create_random_IP();
//    cout<<src_ip_s.data()<<endl;

//    libnet_t *handle; /* LibnetŸä±ú */
//    int packet_size; /* ¹¹ÔìµÄÊýŸÝ°üŽóÐ¡ */

//    char device[]= "ens33"; /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */
//    char src_ip_str[] = "192.168.2.148"; /* ÔŽIPµØÖ·×Ö·ûŽ® */
//    char dst_ip_str[] = "125.216.249.125"; /* Ä¿µÄIPµØÖ·×Ö·ûŽ® */

//    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ÔŽMAC */
//    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ä¿µÄMAC */
//    u_long dst_ip, src_ip; /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
//    char error[LIBNET_ERRBUF_SIZE]; /* ³öŽíÐÅÏ¢ */
//    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ž÷²ãbuildº¯Êý·µ»ØÖµ */
//    u_short proto = IPPROTO_TCP; /* Ž«Êä²ãÐ­Òé */
//    u_char payload[255] = {0}; /* ³ÐÔØÊýŸÝµÄÊý×é£¬³õÖµÎª¿Õ */
//    u_long payload_s = 0; /* ³ÐÔØÊýŸÝµÄ³€¶È£¬³õÖµÎª0 */
//    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
//    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
//    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
//    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
//    /* ³õÊŒ»¯Libnet */

//    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL )
//    {
//        printf("libnet_init failure\n");
//        return (-1);
//    };
//    strncpy((char*)payload, "", sizeof(payload)-1); /* ¹¹ÔìžºÔØµÄÄÚÈÝ */
//    payload_s = strlen((char*)payload); /* ŒÆËãžºÔØÄÚÈÝµÄ³€¶È */

//#if 0
//    /* ¹¹œšTCPµÄÑ¡Ïî,Íš³£ÔÚµÚÒ»žöTCPÍšÐÅ±šÎÄÖÐÉèÖÃMSS */
//    tcp_op_tag = libnet_build_tcp_options(
//                payload,
//                payload_s,
//                handle,
//                0
//    );
//    if (tcp_op_tag == -1) {
//        printf("build_tcp_options failure\n");
//        return (-2);
//    };
//#endif

//    int rand_src_port = rand()%65535;

//    tcp_tag = libnet_build_tcp(
//                (uint16_t)rand_src_port, /* ÔŽ¶Ë¿Ú */
//                (uint16_t)dst_port,      /* Ä¿µÄ¶Ë¿Ú */
//                8888,                    /* ÐòÁÐºÅ */
//                8889,                    /* È·ÈÏºÅ */
//                TH_SYN,                   /* Control flags */
//                14600,                    /* Ž°¿Ú³ßŽç */
//                0,                        /* Ð£ÑéºÍ,0Îª×Ô¶¯ŒÆËã */
//                0,                        /* œôŒ±ÖžÕë */
//                LIBNET_TCP_H + payload_s, /* ³€¶È */
//                payload,                    /* žºÔØÄÚÈÝ */
//                payload_s,                /* žºÔØÄÚÈÝ³€¶È */
//                handle,                    /* libnetŸä±ú */
//                0                        /* ÐÂœš°ü */
//    );

//    if (tcp_tag == -1)
//    {
//        printf("libnet_build_tcp failure\n");
//        return (-3);
//    };

//    /* ¹¹ÔìIPÐ­Òé¿é£¬·µ»ØÖµÊÇÐÂÉú³ÉµÄIPÐ­Òé¿ìµÄÒ»žö±êŒÇ */
//    ip_tag = libnet_build_ipv4(
//        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IPÐ­Òé¿éµÄ×Ü³€,*/
//        0, /* tos */
//        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,Ëæ»ú²úÉú0~65535 */
//        0, /* frag Æ¬Æ«ÒÆ */
//        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,Ëæ»ú²úÉú0~255 */
//        proto, /* ÉÏ²ãÐ­Òé */
//        0, /* Ð£ÑéºÍ£¬ŽËÊ±Îª0£¬±íÊŸÓÉLibnet×Ô¶¯ŒÆËã */
//        src_ip, /* ÔŽIPµØÖ·,ÍøÂçÐò */
//        dst_ip, /* Ä¿±êIPµØÖ·,ÍøÂçÐò */
//        NULL, /* žºÔØÄÚÈÝ»òÎªNULL */
//        0, /* žºÔØÄÚÈÝµÄŽóÐ¡*/
//        handle, /* LibnetŸä±ú */
//        0 /* Ð­Òé¿é±êŒÇ¿ÉÐÞžÄ»òŽŽœš,0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ*/
//    );
//    if (ip_tag == -1) {
//        printf("libnet_build_ipv4 failure\n");
//        return (-4);
//    };
//    /* ¹¹ÔìÒ»žöÒÔÌ«ÍøÐ­Òé¿é,Ö»ÄÜÓÃÓÚLIBNET_LINK */
//    eth_tag = libnet_build_ethernet(
//        dst_mac, /* ÒÔÌ«ÍøÄ¿µÄµØÖ· */
//        src_mac, /* ÒÔÌ«ÍøÔŽµØÖ· */
//        ETHERTYPE_IP, /* ÒÔÌ«ÍøÉÏ²ãÐ­ÒéÀàÐÍ£¬ŽËÊ±ÎªIPÀàÐÍ */
//        NULL, /* žºÔØ£¬ÕâÀïÎª¿Õ */
//        0, /* žºÔØŽóÐ¡ */
//        handle, /* LibnetŸä±ú */
//        0 /* Ð­Òé¿é±êŒÇ£¬0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ */
//    );
//    if (eth_tag == -1) {
//        printf("libnet_build_ethernet failure\n");
//        return (-5);
//    };
//    //before sending
//    packet_size = libnet_write(handle); /* ·¢ËÍÒÑŸ­¹¹ÔìµÄÊýŸÝ°ü*/
//    //after sending
//    libnet_destroy(handle); /* ÊÍ·ÅŸä±ú */
//    return (0);

//}




