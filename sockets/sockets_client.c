#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

//Sockets
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "sockets_utils.h"

#define ErrorExit(x) { perror(x); exit(EXIT_FAILURE); }


int main (int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: binary <num_sec>\n");
        exit(EXIT_FAILURE);
    }
    int sec_value;
    if (sscanf(argv[1], "%d", &sec_value) == 0) {
        printf("Usage: binary <num_sec>\n");
        exit(EXIT_FAILURE);
    }
    printf("You enterd %d seconds\n", sec_value);
    
    //Create socket using TCP
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        ErrorExit("Socket creation");
    }
    
    //Create server address structure
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    
    int rtnVal = inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.s_addr);
    if (rtnVal == 0 ) {
        ErrorExit("Invalid address string");
    } else if (rtnVal < 0) {
        ErrorExit("inet_pton failed");
    }
    servAddr.sin_port = htons(SERVER_PORT); //Server port
    
    
    //Establish connection to the server
    if (connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        ErrorExit("Establishing connection");
    }
    
    int pid = getpid();
    
    ssize_t send_bytes = send(sock, &pid, sizeof(pid), 0);
    if (send_bytes < 0) {
        ErrorExit("send pid");
    } else if (send_bytes != sizeof(pid)) {
        ErrorExit("send unexpected bytes");
    }
    
    send_bytes = send(sock, &sec_value, sizeof(sec_value), 0);
    if (send_bytes < 0) {
        ErrorExit("send sec");
    } else if (send_bytes != sizeof(sec_value)) {
        ErrorExit("send unexpected bytes");
    }
    
    
    //Receive message from server
    fputs("Received from server: ", stdout); // Setup to print the server back string
    char responseBuffer[256];
    ssize_t receive_bytes = recv(sock, responseBuffer, sizeof(responseBuffer), 0);
    if (receive_bytes < 0) {
        ErrorExit("receive failed");
    } else if (receive_bytes == 0) {
        ErrorExit("connection closed permaturely");
    }
    
    fputs(responseBuffer, stdout);
    fputs('\n', stdout);
    

    exit(EXIT_SUCCESS);
}
