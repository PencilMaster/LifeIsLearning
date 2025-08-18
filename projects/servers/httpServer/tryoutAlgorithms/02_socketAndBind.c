// Here we experiment with socket and bind functions a bit.
//
//

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) 
{
    int s, sockfd;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((s = getaddrinfo(NULL, "3490", &hints, &res)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }
    
    for ((struct addrinfo *) it = res; it; it = it->ai_next)
    {
        if ((sockfd = socket(it->ai_family, it->ai_addr, it->ai_addrlen)) == -1) //Should use PF_INET(6) instead of ai_family but it does work otherwise
        {
            fprintf("socket, errno: %d\n", errno);
            return 2;
        }

        if (bind(sockfd, it->ai_addr, it->ai_addrlen) != -1) 
        {
            break;
        }
        fprintf("bind, errno: %d\n", errno);
    }

    
    freeaddrinfo(res);

    return 0;
}
