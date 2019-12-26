#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <pcap.h>



void tcp_packet_callback(unsigned char *argument, const struct pcap_pkthdr *pcap_header,
                            const unsigned char *packet_content) 
{
    struct tcphdr *tcpptr = (struct tcphdr *)(packet_content + 14 + 20);

    printf("----tcp 协议-----\n");
    printf("源端口: %d\n", ntohs(tcpptr->source));
    printf("目的端口: %d\n", ntohs(tcpptr->dest));

    printf("序列号: %u\n", ntohl(tcpptr->seq));
    printf("确认号: %u\n", ntohl(tcpptr->ack_seq));
    printf("首部长度: %d\n", tcpptr->doff*4);
    printf("检验和: %d\n", ntohs(tcpptr->check));
    printf("窗口大小: %d\n", ntohs(tcpptr->window));
    printf("紧急指针: %d\n", ntohs(tcpptr->urg_ptr));

    return;
}

void ip_packet_callback(unsigned char *argument, const struct pcap_pkthdr *pcap_header, const unsigned char *packet_content) 
{
    struct in_addr s, d;
    struct iphdr *ipptr;
    ipptr = (struct iphdr *)(packet_content + 14);

    printf("-----IP 协议 (网络层)-----\n");
    printf("版本号: %d\n", ipptr->version);
    printf("首部长度: %d\n", ipptr->ihl*4);
    printf("区分服务: %d\n", ipptr->tos);
    printf("总长度: %d\n", ntohs(ipptr->tot_len));
    printf("标识: %d\n", ntohs(ipptr->id));
    printf("片偏移: %d\n", ntohs((ipptr->frag_off & 0x1fff) * 8));
    printf("TTL: %d\n", ipptr->ttl);
    printf("检验和: %d\n", ntohs(ipptr->check));
    printf("协议: %d\n", ipptr->protocol);
    s.s_addr=ipptr->saddr;
    d.s_addr=ipptr->daddr;
    printf("源地址: %s\n", inet_ntoa(s));
    printf("目的地址: %s\n", inet_ntoa(d));

    switch(ipptr->protocol) {
    case 6:
        printf("tcp 协议\n");
        tcp_packet_callback(argument, pcap_header, packet_content);
        break;
    case 1:
        printf("icmp 协议\n");
        break;
    case 17:
        printf("udp 协议\n");
        break;
    default:
        break;
    }

    return;
}

void arp_packet_callback(unsigned char *argument, const struct pcap_pkthdr *pcap_header,
                const unsigned char *packet_content) 
{
    printf("------ARP 协议-------\n");

    return;
}

void ethernet_packet_callback(unsigned char *argument, const struct pcap_pkthdr *pcap_header,
                        const unsigned char *packet_content) 
{
    struct ethhdr *ethptr;
    struct iphdr *ipptr;
    unsigned char *mac;

    ethptr=(struct ethhdr *)packet_content;
    printf("\n---以太网 协议(物理层)-----\n");
    printf("MAC 源地址:\n");
    mac = ethptr->h_source;
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *mac, *(mac+1), *(mac+2), *(mac+3), *(mac+4), *(mac+5));
    printf("MAC 目的地址:\n");
    mac = ethptr->h_dest;
    printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *mac, *(mac+1), *(mac+2), *(mac+3), *(mac+4), *(mac+5));
    printf("协议: %04x\n", ntohs(ethptr->h_proto));

    switch(ntohs(ethptr->h_proto)) {
    case 0x0800:
        printf("这是 IP 协议\n");
        ip_packet_callback(argument,pcap_header,packet_content);
        break;
    case 0x0806:
        printf("这是 ARP 协议\n");
        arp_packet_callback(argument,pcap_header,packet_content);
        break;
    case 0x8035:
        printf("这是 RARP 协议\n");
        break;
    default:
        break;

    }

    return;
}

int main(int argc, char **argv) 
{
    pcap_t *pt;
    char *dev;
    char errbuf[128];
    struct bpf_program fp;
    bpf_u_int32 maskp, netp;
    int ret,i=0,inum;
    int pcap_time_out = 5;
    char filter[128] = {0};
    unsigned char *packet;
    struct pcap_pkthdr hdr;
    pcap_if_t *alldevs,*d;

    if(pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr,"find interface failed!\n");
        return -1;
    }

    for(d = alldevs; d; d = d->next) {
        printf("%02d. %s", ++i, d->name);
        if(d->description)
            printf("(%s)\n", d->description);
        else
            printf("(no description available)\n");
    }

    if(i == 1) {
        dev = alldevs->name;
    } else {
        printf("input a interface(1-%d):", i);
        scanf("%d", &inum);
        if(inum < 1 || inum > i) {
            printf("interface number out of range\n");
            return -1;
        }

        for(d = alldevs, i = 1; i<inum; d = d->next, i++);

        dev = d->name;
    }

    /*
    dev=pcap_lookupdev(errbuf);
    if(dev==NULL){
         fprintf(stderr,"%s/n",errbuf);
         return;
    }
    */

    printf("dev: %s\n", dev);
    ret = pcap_lookupnet(dev, &netp, &maskp, errbuf);
    if(ret == -1) {
        fprintf(stderr,"%s\n", errbuf);
        return -1;
    }

    pt = pcap_open_live(dev, BUFSIZ, 1, pcap_time_out, errbuf);
    if(pt == NULL) {
        fprintf(stderr,"open error :%s\n", errbuf);
        return -1;
    }
    
    if(pcap_compile(pt, &fp, filter, 0, netp) == -1) {
        fprintf(stderr, "compile error\n");
        return -1;
    }

    if(pcap_setfilter(pt, &fp) == -1) {
        fprintf(stderr,"setfilter error\n");
        return -1;
    }
    pcap_loop(pt, -1, ethernet_packet_callback, NULL);

/*    
    while(1) {
         printf("wait packet:filter %s/n",filter);
         packet=(char *)pcap_next(pt,&hdr);
         if(packet==NULL)
              continue;
         else
              printf("get a packet/n");
    }
*/
    
    pcap_close(pt);

    return 0;
}
