
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>


#define ErrorExit(x) { perror(x); exit(EXIT_FAILURE); }

int main (int argc, char **argv) {
    
    int fds[2]; //File descriptors
    if (pipe(fds) == -1) {
        ErrorExit("pipe define");
    }
    
    int fpid;
    
    
    //Switch for ls
    switch (fpid = fork()) {
        case -1:
            ErrorExit("[ls] fork 1");
        
        case 0:
            //Child
            
            printf("[ls] fpid: %d, pid: %d, ppid: %d\n", fpid, getpid(), getppid());
            close(fds[0]); //close read descriptor
            
            if (dup2(fds[1], STDOUT_FILENO) == -1) {
                ErrorExit("[ls] dup2");
            }
            
            printf("[ls] fpid: %d, pid: %d, ppid: %d, before execlp()\n", fpid, getpid(), getppid());
            if (execlp("ls", "ls", (char*)NULL)) {
                ErrorExit("[ls] execlp");
            }
                
            //this code never will be called
            //....
            exit(EXIT_SUCCESS);
            
        default:
            //Parent
            printf("[ls] Parent: fpid: %d, pid: %d, ppid: %d, after fork()\n", fpid, getpid(), getppid());
//            break;
    }
    
    
    //Switch for wc
    switch (fpid = fork()) {
        case -1:
            ErrorExit("[wc] fork 1");
        
        case 0:
            //Child
            printf("[wc] fpid: %d, pid: %d, ppid: %d\n", fpid, getpid(), getppid());
            close (fds[1]); //Close write descriptor
                    
            if (dup2(fds[0], STDIN_FILENO) == -1) {
                ErrorExit("[wc] dup2");
            }
                    
            //close(fds[0])
            //if (execlp("ls", "ls", "-l", "-a", (char*)NULL)) {
            if (execlp("wc", "wc", (char*)NULL)) {
                ErrorExit("[wc] execlp");
            }
            
                    
            //this code never will be called
            //....
            exit(EXIT_SUCCESS);
            
        default:
            //Parent
            printf("[wc] Parent: fpid: %d, pid: %d, ppid: %d, after fork()\n", fpid, getpid(), getppid());
//            break;
    }
    
    //Close both file descriptors
    close(fds[0]);
    close(fds[1]);
    
    //Waiting ls
    wait(NULL);
    //Waiting wc
    wait(NULL);
    
}
