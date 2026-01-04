/*
 * MEHDI NAOUI
 * IFNETLIST
 * 2025
 * https://github.com/Mehdji
 * List network interfaces and show Name, Ip address and Netmask address.
 * */

//Libraries
#define _GNU_SOURCE
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/if_link.h>
#include <net/if.h>

//Constant
#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define IFACE "\033[1;34m"
#define RESET "\033[0m"

//Entry point
int main(int argc, char const *argv[])
{
    //Create a pointer to an ifaddrs struct
    struct ifaddrs *ifaddr;
    
    int family, s, s1;
    char host [NI_MAXHOST];
    char netmask [NI_MAXHOST];

    //Test if getifaddrs return an error code
    if (getifaddrs(&ifaddr) == -1){
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    
    //Iterate over network interfaces
    for(struct ifaddrs *ifa = ifaddr; ifa!=NULL ;ifa = ifa->ifa_next){
        if(ifa->ifa_addr == NULL)
            continue;


            family = ifa->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6){
            //printf("─────────────────────────────────────────");
            //family = ifa->ifa_addr->sa_family;

            printf("\n───•" BOLD IFACE "%-8s" RESET "───────────────\n",
               ifa->ifa_name);

            printf("\t↳Family: %s\n",
               (family == AF_PACKET) ? "AF_PACKET" :
               (family == AF_INET) ? "IPv4" :
               (family == AF_INET6) ? "IPv6" : "???"
               );
            printf("\t↳Status : %s", (ifa->ifa_flags & IFF_UP) ? GREEN"✔UP"RESET : RED"DOWN"RESET);
            


        //if (family == AF_INET || family == AF_INET6){
            s = getnameinfo(ifa->ifa_addr,(family == AF_INET) ? sizeof(struct sockaddr_in):sizeof(struct sockaddr_in6),
                 host, NI_MAXHOST,
                 NULL, 0, NI_NUMERICHOST);

            //If getname return an error code print a message and exit
            if(s !=0){
                printf("getnameinfo() failed: %s\n",gai_strerror(s));
                exit(EXIT_FAILURE);
            }

            printf("\n\t↳Address: "BOLD"%s"RESET"\n", host);

            s1 = getnameinfo(ifa->ifa_netmask,(family == AF_INET) ? sizeof(struct sockaddr_in):
                sizeof(struct sockaddr_in6),
                netmask, NI_MAXHOST,
                NULL, 0, NI_NUMERICHOST);
        
            if (family == AF_INET){
                printf("\t↳Netmask: "BOLD"%s"RESET"\n", netmask);
            }
    
        /*
        }else if (family == AF_PACKET&& ifa->ifa_data != NULL){
            struct rtnl_link_stats *stats = ifa->ifa_data;

            printf("\n\t↳tx_packets = %10u; rx_packets = %10u\n"
               "\t↳tx_bytes = %10u; rx_bytes = %10u\n",
               stats->tx_packets, stats->rx_packets,
               stats->tx_bytes, stats->rx_bytes);
        }
        */
    }
    }

freeifaddrs(ifaddr);
exit(EXIT_SUCCESS);
return 0;

    
}

