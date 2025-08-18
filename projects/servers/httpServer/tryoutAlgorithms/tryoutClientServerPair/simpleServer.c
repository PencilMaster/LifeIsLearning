// Here we manually write in the server Code from Beej's guide.
// It is about a stream socket server demo.
//

// comprehensive list of includes using POSIX define
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>          // fprintf, perror, printf
#include <stdlib.h>         // exit
#include <string.h>         // memset
#include <errno.h>          // errno

#include <unistd.h>         // close, fork
#include <sys/types.h>      // pid_t, socklen_t
#include <sys/socket.h>     // socket, bind, listen, accept, setsockopt, send
#include <netdb.h>          // getaddrinfo, freeaddrinfo, gai_strerror
#include <arpa/inet.h>      // inet_ntop, INET6_ADDRSTRLEN
#include <netinet/in.h>     // sockaddr_in, sockaddr_in6

#include <signal.h>         // sigaction, sigemptyset, SIGCHLD, SA_RESTART
#include <sys/wait.h>       // waitpid, WNOHANG

#define PORT "3490" // this defines the port that users will connect to
#define BACKLOG 10 // this defines how many pending connections the queue will hold.

// here we implement the sigchld_handler. PID is > 0 for main process and 0 for child process.
void sigchld_handler(int s)
{
    (void)s; // to quiet unused variable warning

    // waitpid() might overwrite errno, so we save it here
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);

}
 
int main(void) 
{
    // listen on sockfd, new connections on new_fd
    int sockfd, new_fd;
    struct addrinfo hints, *serverinfo, *p;

    // connector's address info, unknown whether IPv4 or IPv6
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

    // Not sure about the use of sigaction, TODO: Research.
    struct sigaction sa;

    // yes should be used by setsockopt to allow the reuse of a port
    int yes = 1;
    
    // this will hold the IP address in presentation mode.
    char s[INET6_ADDRSTRLEN];
    int rv;

    // first we make sure the hints struct is cleared and fill it with wanted hints.
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;          // Makes sure IPv4 is used for this socket
    hints.ai_socktype = SOCK_STREAM;    // Makes sure that bind() only returns valid if port is also a stream_socket
    hints.ai_flags = AI_PASSIVE;        // Makes sure it uses our own IP
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &serverinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    // loop through all the results and bind to the first socket we can
    for (p = servinfo; p; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        // before bind, we want to make sure the port can be reused if a socket connection is still hanging around in the kernel
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        // now try to bind the current sock_addr to the socket, otherwise try with the next one in the linked list.
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }
    
    // at this point we don't need the sock_addr linked list anymore.
    freeaddrinfo(servinfo);

    // if p == NULL then we reached the end of all sock_addr structs, failing to bind any of them. Try to start the server again.
    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    // try to start listening on the bound port.
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;    // this removes all dead / exited processes.
    sigemptyset(&sa.sa_mask);           // this set contains signals which are blocked while handler runs. Here none.
    sa.sa_flags = SA_RESTART;           // without this, accept/read/write can fail constantly on EINTR.

    // this is an extended version of the handler. Also gathers information on sender.
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1)
    {
        sin_size = sizeof(their_addr);
        if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }

        // we defines the following function above the main function.
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof(s));
        printf("server got connection from: %s\n", s);

        // different thatn Beej does it, but from research it looks more secure.
        int pid = fork();
        if (!pid)
        {
            close(sockfd); // listener is not needed by child.
            if (send(new_fd, "Hello World!", 13, 0) == -1)
            {
                perror("send");
            }
            close(new_fd);
            exit(0);
        }
        else if (pid < 0)
        {
            perror("fork");
        }
        close(new_fd); // parent does not need connection anymore
    }

    return 0;
}
