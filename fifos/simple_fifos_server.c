#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>


#include "fifos_utils.h"


#define ErrorExit(x) { perror(x); exit(EXIT_FAILURE); }

int main(int argc, char** argv) {
    char buf[PIPE_BUF];
    
    
    if (mkfifo(simple_fifo_server_name, 0x666) == -1) {
        ErrorExit("mkfifo");
    }
    
    int fd = open(simple_fifo_server_name, O_RDONLY);
    if (fd == -1) {
        ErrorExit("open");
    }
    
    
    int client_pid;
//    dprintf(STDOUT_FILENO, "%s", "Server started...");
    for (;;) {
        ssize_t bytes_read = read(fd, &client_pid, sizeof(client_pid));
        if (bytes_read == 0) {
            continue;
        }
        if (bytes_read != sizeof(client_pid)) {
            ErrorExit("read error");
        }
        printf("client_pid: %d\n", client_pid);
    }
    
    if (unlink(simple_fifo_server_name) == -1) {
        ErrorExit("unlink");
    }
    exit(EXIT_SUCCESS);
}
