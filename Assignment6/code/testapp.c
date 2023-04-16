#include <linux/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define CDRV_IOC_MAGIC 'Z'
#define E2_IOCMODE1 _IO(CDRV_IOC_MAGIC, 1)
#define E2_IOCMODE2 _IO(CDRV_IOC_MAGIC, 2)

char* filename = "/dev/a5";

int case_1()
{
    int fd_a, fd_b;   
    if(fork() == 0)
    {
        // Introduce slight delay to ensure that the parent process opens the device first
        usleep(10*1000);
        printf("Open fd b\n");
        fd_b = open(filename, O_RDWR);
        close(fd_b);
    }
    else{
        printf("Open fd a\n");
        fd_a = open(filename, O_RDWR);
        // Add slight delay to ensure that the child process can enter the open call
        usleep(30*1000);
        printf("Change IO mode from 1 to 2\n");
        ioctl(fd_a, E2_IOCMODE2);
        // It will get stuck here due to a deadlock created by count1 being greater than 1 
        // but fd_b being restricted from moving out of the open by sem2 being held by fd_a
        printf("Mode was changed from 1 to 2\n");
        close(fd_a);
    }
    return 1;   
}


int case_2()
{
    // There is a deadlock created by opening the file twice in the same process
    // This is due to a deadlock create by sem2 being held by fd_a and will not be released until it is closed
    // The open should return that the file cannot be accessed instead of introducing this deadlock
    int fd_a, fd_b;   
    fd_a = open(filename, O_RDWR);
    fd_b = open(filename, O_RDWR);
    close(fd_a);
    close(fd_b);
    return 1;
}

int main(int argc, char* argv[])
{
    //case_1();
    case_2();
    return 0;   
}