#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pcap.h>  

#define MAXBYTE2CAPTURE 65535  
      
void processPacket(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet) 
{  
    int *counter = (int *)arg;  
      
    printf("Packet Count: %d\n", ++(*counter));  
    printf("Received Packet Size: %d\n", pkthdr->len);  
    printf("Payload:\n");

    unsigned int i = 0;
    for (i = 0; i < pkthdr->len; i++) {  
        if (isprint(packet[i]))  
            printf("%c ", packet[i]);  
        else   
            printf(". ");
      
        if ((i % 16 == 0 && i != 0) || i == pkthdr->len-1)  
            printf("\n");    
    }

    return;  
}  
      
int main() 
{
    pcap_t *descr = NULL;  
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    char *device = NULL;
     
    /* Get the name of the first device suitable for capture */  
    device = pcap_lookupdev(errbuf);  
      
    printf("Opening device %s\n", device);  
      
    /* Open device in promiscuous mode */  
    descr = pcap_open_live(device, MAXBYTE2CAPTURE, 1, 0, errbuf);  
    if(!descr)
    {
        perror("pcap_open_live");
        exit(1);
    }
      
    int count = 0;  
    /* Loop forever & call processPacket() for every received packet */  
    pcap_loop(descr, -1, processPacket, (u_char *)&count);  
      
    return 0;
}
