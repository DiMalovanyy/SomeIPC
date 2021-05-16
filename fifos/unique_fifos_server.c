#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>


#include "fifos_utils.h"

#define USERS_SIZE 1000
#define ExitError(x) { perror(x); exit(EXIT_FAILURE); }

struct User {
    int pid;
    int unique_num;
};

//Return -1 if user does not exist
int getUserUniqueId(struct User* users, size_t size, int pid) {
    for (int i = 0; i < size; ++i) {
        if (users[i].pid == pid) {
            return users[i].unique_num;
        }
    }
    return -1;
}

int main(int argc, char ** argv) {
    struct User users[USERS_SIZE];
    int current_size = 0;

    if (mkfifo(unique_fifo_request, 0x666) == -1) {
        ExitError("mkfifo request");
    }
    
    if (mkfifo(unique_fifo_response, 0x666) == -1) {
        ExitError("mkfifo response");
    }

    int request_fd = open(unique_fifo_request, O_RDONLY);  //open fifo descriptor for the requests
    int response_fd = open(unique_fifo_response, O_WRONLY); //open fifo descriptor for the response
    if (request_fd == -1) {
        ExitError("open request");
    }
    if (response_fd == -1) {
        ExitError("open response");
    }

    
    //there are two commands
    //set <pid>      --set unique num for client dummy id (process id)
    //get <pid>      --get unique num to client
    for (;;) {
        char read_buf[PIPE_BUF];
        ssize_t bytes_read = read(request_fd, read_buf, sizeof(read_buf));
        if (bytes_read == 0) {
            continue;
        }

//        sleep(5);
        //do some work
        //...
        
        //Parse dummy request
        int pid;
        char request[4];
        sscanf(read_buf, "%s%d", request, &pid);
        request[3] = '\0';
        dprintf(STDOUT_FILENO, "Request: %s; Pid: %d;\n", request, pid);
        
        
        //Process response
        char responseBuf[PIPE_BUF];
        int res = getUserUniqueId(users, current_size, pid);
        if (strcmp(request, "get") == 0) {
            if (res == -1 ) {
                sprintf(responseBuf, "%s", "No user with this id\n");
            } else {
                sprintf(responseBuf, "User found with id: %d\n", res);
            }
        } else { // request = set
            if (res != -1) {
                sprintf(responseBuf, "%s", "User already has unique id\n");
            } else {
                //Dummy seted unique id = index in array of users
                struct User current_user = { pid , current_size++ };
                sprintf(responseBuf, "Successfully seted value: %d\n", current_size - 1);
            }
        }
        dprintf(STDOUT_FILENO, "Response: %s", responseBuf);
        if (write(response_fd, responseBuf, sizeof(responseBuf)) == -1) {
            ExitError("write response");
        }
        
    }

    //Never executed ... because progrm ended by termination
    if (unlink(unique_fifo_request) == -1) {
        ExitError("unlink request");
    }
    if (unlink(unique_fifo_response) == -1) {
        ExitError("unlink response");
    }

    exit(EXIT_SUCCESS);
}
