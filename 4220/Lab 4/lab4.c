#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
    char readIn;
    int fd = open("/tmp/N_pipe1", O_RDONLY);
    while(1){
        int numb = read(fd, &readIn, sizeof(readIn));
        printf("%d\n", read);

    }
    
}
