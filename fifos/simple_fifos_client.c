#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>


#include <sys/types.h>
#include <sys/stat.h>

#include "fifos_utils.h"


#define ErrorExit(x) { perror(x); exit(EXIT_FAILURE); }

int main(int argc, char** argv) {
    
    
    int fd = open(simple_fifo_server_name, O_WRONLY);
    if (fd == -1) {
        ErrorExit("Open");
    }
    
    int pid = getpid();
    printf("fd: %d, pid: %d\n", fd, pid);
    
    if (write(fd, &pid, sizeof(pid)) == -1) {
        ErrorExit("write");
    }
    
    exit(EXIT_SUCCESS);
}
