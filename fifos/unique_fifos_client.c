#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "fifos_utils.h"
#define ErrorExit(x) { perror(x); exit(EXIT_FAILURE); }
int main(int argc, char ** argv) {
    //Parse request
    if (argc < 2) {
        printf("call schema: <set>/<get>\n");
        exit(EXIT_FAILURE);
    }
    
    char request[4];
    strncpy(request, argv[1], 3);
    request[3] = '\0';
    
    if ((strcmp(request, "set") != 0
        && strcmp(request, "get") != 0) || strlen(argv[1]) != 3) {
        printf("invalid request: <set>/<get>\n");
        exit (EXIT_FAILURE);
    }
    printf("Your request: %s\n", request);
    
    
    
    //processing request
    int request_fd = open(unique_fifo_request, O_WRONLY);
    if (request_fd == -1) {
        ErrorExit("request open");
    }
    
    int current_pid = getpid();
    printf("pid: %d\n", current_pid);
    
    
    char write_buf[PIPE_BUF];
    sprintf(write_buf, "%s %d", request, current_pid);
    
    if (write(request_fd, write_buf, sizeof(write_buf))  == -1) {
        ErrorExit("request write");
    }
    
    
    //Get response
    int response_fd = open(unique_fifo_response, O_RDONLY);
    if (response_fd == -1) {
        ErrorExit("response open");
    }
    char read_buf[PIPE_BUF];
    ssize_t bytes_read = read(response_fd, read_buf, sizeof(read_buf));
    if (bytes_read <= 0) {
        ErrorExit("response read");
    }
    dprintf(STDOUT_FILENO, "Response: %s", read_buf);
    
    exit(EXIT_SUCCESS);
}
