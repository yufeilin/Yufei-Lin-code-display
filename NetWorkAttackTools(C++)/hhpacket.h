#ifndef HHPACKET_H
#define HHPACKET_H



class packet{
public:
    char* src_mac,*dst_mac;
    char* src_ip,*dst_ip;
    int src_port,dst_port;
    char* description;
    char* protocol;
    int length;
    char* time;

    packet(int x){}
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



#endif // HHPACKET_H
