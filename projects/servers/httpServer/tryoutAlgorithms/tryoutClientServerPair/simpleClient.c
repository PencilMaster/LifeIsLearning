// Here we write down the Client code manually from Beej's guide
//
// all include statements with a POSIX define and clear comments. These were done by gpt and checked by me
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>          // fprintf, printf, perror
#include <stdlib.h>         // exit
#include <string.h>         // memset
#include <errno.h>          // errno (if you inspect it)

#include <unistd.h>         // close
#include <sys/types.h>      // socklen_t
#include <sys/socket.h>     // socket, connect, recv
#include <netdb.h>          // getaddrinfo, freeaddrinfo, gai_strerror
#include <arpa/inet.h>      // inet_ntop, INET6_ADDRSTRLEN
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6
                            //
#define PORT "3490"         // port the client will connect to. And server is listening on
#define MAXDATASIZE 100     // max number of bytes we can receive at once

// typical transformation of IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa->sin6_addr);
}

int main(int argc, char *argv[])
{
    // socket descriptor and number of bytes received
    int sockfd, numbytes;
    char buf[MAXDATASIZE];

    struct addrinfo hints, *serverinfo, *p;
    int rv;         // contains errorvalue returned by getaddrinfo()
    char s[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "usage: client hostname unspecified\n");
        exit(1);
    }

    // we set wipe memory of hints struct to be safe
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &serverinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // addrinfo linked list is filled, now we loop over all results and connect to first viable.
    for (p = serverinfo; p; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        // interesting how he does not use setsockopt here to make ports reusable.
        if (inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s)) == NULL)
        {
            perror("client: inet_ntop");
            continue;
        }
        printf("client attempting to connect to: %s\n", s);

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("client: connect");
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    // here he uses inet_ntop() again, but we already did it above. I will try without doing it again.
    printf("client: connected to %s\n", s);

    freeaddrinfo(serverinfo);

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
    {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';
    printf("client: received '%s'\n", buf);

    close(sockfd);

    return 0;
}
