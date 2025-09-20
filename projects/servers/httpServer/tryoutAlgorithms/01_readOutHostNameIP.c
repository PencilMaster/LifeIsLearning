// I added code from the book, line by line with small adjustments as a tryout.

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) 
{
    struct addrinfo hints, *res, *p;
    int status, portNum;
    char ipstr[INET6_ADDRSTRLEN];
    
    if (argc != 2)
    {
        fprintf(stderr, "usage: showip hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if ((status = getaddrinfo(argv[1], "https", &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
    
    printf("IP addresses for %s:\n\n", argv[1]);

    for (p = res; p != NULL; p = p->ai_next)
    {
        void *addr;
        char *ipver;
        struct sockaddr_in *ipv4;
        struct sockaddr_in6 *ipv6;

        if (p->ai_family == AF_INET)
        {
            ipv4 = (struct sockaddr_in *) p->ai_addr;
            addr = &(ipv4->sin_addr);
            portNum = ipv4->sin_port;
            ipver = "IPv4";
        }
        else
        {
            ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            addr = &(ipv6->sin6_addr);
            portNum = ipv6->sin6_port;
            ipver = "IPv6";
        }

        // Now convert IP address from network to presentation (string) and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf(" %s: %s, Portnumber: %d\n", ipver, ipstr, ntohs(portNum));
    }

    freeaddrinfo(res);

    return 0;
}
