#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <pthread/pthread.h>

#include <sys/types.h>
#include <sys/stat.h>

//Sockets
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "sockets_utils.h"

#define ErrorExit(x) { perror(x); exit(EXIT_FAILURE); }

//Implementation of some work
void SomeWork(int secconds) {
    dprintf(STDOUT_FILENO, "Some work begin...");

    sleep(secconds);
    
    dprintf(STDOUT_FILENO, "Some work end!");
}

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main( int argc, char **argv) {
    
    int servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock < 0) {
        ErrorExit("socket creation");
    }
    
    // Construct local address structure
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERVER_PORT);
    
    // Bind to the local address
    if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {
        ErrorExit("bind");
    }
    
    
    // Mark the socket so it will listen for incoming connections
    if (listen(servSock, MAXPENDING) < 0) {
        ErrorExit("listen");
    }
    
    for (;;) {
        struct sockaddr_in clntAddr; // Client address
        socklen_t clntAddrLen = sizeof(clntAddr);
        
        // Wait for a client to connect
        int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
        if (clntSock < 0) {
            ErrorExit("client connection");
        }
        
        // clntSock is connected to a client!
        char clntName[INET_ADDRSTRLEN]; // String to contain client address
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL) {
            printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
        } else {
            puts("Unable to get client address");
        }
        
        int client_pid, secconds;
        int bytes_read;
        bytes_read = read(clntSock, &client_pid, sizeof(client_pid));
        if (bytes_read < 0) {
            ErrorExit("read pid failed");
        } else if (bytes_read != sizeof(client_pid)) {
            ErrorExit("read unexpected bytes");
        }
        
        bytes_read = read(clntSock, &secconds, sizeof(secconds));
        if (bytes_read < 0) {
            ErrorExit("read secconds failed");
        } else if (bytes_read != sizeof(secconds)) {
            ErrorExit("read unexpected bytes");
        }
        
        pthread_t threadId;
        int err = pthread_create(&threadId, NULL, &SomeWork, secconds);
        if (err) {
            printf("Client with pid %d not started", client_pid);
        } else {
            printf("Client with pid %d started ...", client_pid);
        }
    }
    
    exit (EXIT_SUCCESS);
}
