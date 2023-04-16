#include <linux/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
    int fd_a, fd_b;
    int child;
    char* filename = "/dev/a5";
    child = fork();
    
    if(child == 0)
    {
        printf("Child started\n");
        //usleep(10*1000);
        fd_b = open(filename, O_RDWR);
        printf("Child opened file\n");
        close(fd_b);
        printf("Child closed file\n");
    }
    else
    {
        fd_a = open(filename, O_RDWR);
        ioctl(fd_a, 2);
        usleep(300*1000);
        close(fd_a);
    }
    wait(NULL);
    return 0;   
}