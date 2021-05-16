#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>


//inline void Sync1(int argc) {
//    for (int i = 0; i < argc; ++i) {
//        wait(NULL); //Wait until all (child) procces ended
//    }
//}

int main(int argc, char ** argv) {
    
    printf("%s, pid: %d, ppid: %d\n", "Start", getpid(), getppid());
    
    int fds[2]; //File descriptors
    
    if (pipe(fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    //switch deffinition
    int fpid;
    int sec;
    //------------------
    
    for ( int i = 1; i < argc; ++i ) {
        switch (fpid = fork()) {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0: //Child case
                
                //Here some work
                //....
                
                //In my case dummy work
                sec = atoi(argv[i]);
                printf("fpid: %d, pid: %d, ppid: %d, sleeping for %d seconds...\n", fpid, getpid(), getppid(), sec);
                sleep(sec);
                printf("fpid: %d, pid: %d, ppid: %d, waking up...\n", fpid, getpid(), getppid());
                
                
                
                //Syncronize parent (for sync 2)
                if (close(fds[1]) == -1) {
                    perror("close fds[1] child");
                    exit(EXIT_FAILURE);
                } //Close write descriptor
                
                //Now child can do other things that wont be syncronized with
                //parent this means that parent can be close when this work will processing
                //Some work
                
                
                exit(EXIT_SUCCESS);
                
            default: //Parent
                printf("Parent: fpid: %d, pid: %d, ppid: %d, after fork()\n", fpid, getpid(), getppid());
        }
    }
    
    
    //First type syncronization
    //Sync1(argc);
    
    //Second sync Type
    
    if( close(fds[1]) == -1) {
        perror("Close fds[1] parent");
        
    } //Close write descriptor
    char dummy;
    if (read(fds[0], &dummy, 1) != 0 ) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    printf("fpid: %d, pid: %d, ppid: %d, after sync\n", fpid, getpid(), getppid());
    
    exit(EXIT_SUCCESS);
}
