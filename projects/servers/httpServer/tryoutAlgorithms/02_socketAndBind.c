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

    if ((sockfd = socket(res->ai_family, res->ai_addr, res->ai_addrlen)) == -1)
    {
        fprintf("socket, errno: %s\n", errno);
        return 2;
    }



    return 0;
}
