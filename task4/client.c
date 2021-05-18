#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "fifo_utils.h"

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
    
    
    int request_fd  = open(fifo_request, O_WRONLY);
    if (request_fd == -1) {
        ErrorExit("open");
    }
    
    int pid = getpid();
    printf("Current pid: %d", pid);
    
    //Just write to server pid
    //Server will created client fifos with that id
    if (write(request_fd, &pid, sizeof(pid)) == -1) {
        ErrorExit("write pid");
    }
    
    //Write data about client fifo
    char path_buffer[512];
    sprintf(path_buffer, fifo_client, pid);
    
    int current_fd = open(fifo_client, O_RDWR);
    if (current_fd == -1) {
        ErrorExit("client fifo");
    }
    
    //write in esstablished client fifo seconds to wait
    if (write(current_fd, &sec_value, sizeof(sec_value)) == -1) {
        ErrorExit("seconds write");
    }
    
    //Get some response
    char read_buffer[PIPE_BUF];
    ssize_t readed_bytes = read(current_fd, read_buffer, sizeof(read_buffer));
    if (readed_bytes == 0) {
        ErrorExit("read respond");
    }
    
    printf("Respons from server: %s", read_buffer);

    exit(EXIT_SUCCESS);
}
