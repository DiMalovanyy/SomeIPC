
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>


#include <sys/types.h>
#include <sys/stat.h>

#include <pthread/pthread.h>

#include "fifo_utils.h"


#define ErrorExit(x) { perror(x); exit(EXIT_FAILURE); }


//Implementation of some work
void SomeWork(int client_pid) {
    dprintf(STDOUT_FILENO, "Some work begin...");
    char path_buffer[512];
    sprintf(path_buffer, fifo_client, client_pid);
    if (mkfifo(fifo_request, 0x666) == -1) {
        ErrorExit("mkfifo client");
    }
    
    int client_fd = open(path_buffer, O_RDWR);
    if (client_fd == -1) {
        ErrorExit("client fifo");
    }
    int secconds;
    ssize_t readed_bytes = read(client_fd, &secconds, sizeof(secconds));
    if (readed_bytes != sizeof(secconds)) {
        ErrorExit("client read error");
    }
    sleep(secconds);
    
    if (write(client_fd, "OK", sizeof("OK")) == 0) {
        ErrorExit("client write respond");
    }
    
    dprintf(STDOUT_FILENO, "Some work end!");
    remove(path_buffer);
}


int main( int argc, char **argv) {
    
    
    if (mkfifo(fifo_request, 0x666) == -1) {
        ErrorExit("mkfifo");
    }
    int request_fd = open(fifo_request, O_RDONLY);
    if (request_fd == -1 ) {
        ErrorExit("open");
    }
    
    printf("Server start listening connections...");
    int client_pid;
    for (;;) {
        ssize_t readed_bytes = read(request_fd, &client_pid, sizeof(client_pid));
        if (readed_bytes == 0) {
            continue;
        }
        if (readed_bytes != sizeof(client_pid)) {
            ErrorExit("read pid");
        }
        
        pthread_t client_thread;
        int thread_creation_error = pthread_create(&client_thread, NULL, &SomeWork, client_pid);
        if (thread_creation_error == 0) {
            ErrorExit("thread creatuin")
        }
    }
    
    if (unlink(fifo_request) == -1 ) {
        ErrorExit("unlink");
    }
    
    exit (EXIT_SUCCESS);
}
