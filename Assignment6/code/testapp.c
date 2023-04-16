#include <linux/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

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
        // The wait on line 154 is partially responsible for this deadlock
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
    // Caused by line 49
    int fd_a, fd_b;   
    fd_a = open(filename, O_RDWR);
    fd_b = open(filename, O_RDWR);
    close(fd_a);
    close(fd_b);
    return 1;
}

int case_3()
{
    // There is a deadlock when trying to switch from Mode 2 to Mode 1 when there are several dependant processes open
    // In this case the child process will try to change the Mode from 2 to 1
    // The process will get stuck in an infinite loop at line 74 until the other process releases its fd
    // Since the child is initiating the infinte loop it prevents the parent from releasing and there is a deadlock
    int fd_a, fd_b, fd_c;   
    // Open the file and change the driver to run in Mode 2
    fd_a = open(filename, O_RDWR);
    ioctl(fd_a, E2_IOCMODE2);
    close(fd_a);

    if(fork()==0)
    {
        fd_b = open(filename, O_RDWR);
        usleep(10*1000);
        ioctl(fd_a, E2_IOCMODE1);
        close(fd_b);
    }
    else
    {
        usleep(1*1000);
        fd_c = open(filename, O_RDWR);
    }
    wait(NULL);
    close(fd_c);
    return 1;
}

int case_4()
{
    // There is a deadlock created where two files are trying to fight over the Mode 
    // This deadlock is due to the fact that the Sem1 and 2 values get into a deadlock
    // with eachother over the different mode swaps
    int fd_a, fd_b;
    int child, value;
    char* string_text = "NewText";
    child = fork();
    if(child == 0)
    {
        fd_a = open(filename, O_RDWR);
        for(int i=0;i<100;i++)
        {
            value = ioctl(fd_a, E2_IOCMODE2, 0);
            value = ioctl(fd_a, E2_IOCMODE1, 0);
        }
    }
    else
    {
        fd_b = open(filename, O_RDWR);
        for(int i=0;i<100;i++)
        {
            value = ioctl(fd_b, E2_IOCMODE2, 0);
            value = ioctl(fd_b, E2_IOCMODE1, 0);
        }
        wait(NULL);
        printf("Child has returned\n");
    }
    return 1;
}

int main(int argc, char* argv[])
{
    // Make sure to run the ./reset.sh script in between runs to reset the state of the driver
    // If the driver can not be reset then a full system reboot will be required
    // This will usually require a forced power down
    if(argc < 2)
        return 0;
    switch(atoi(argv[1])) {
    case 1:
        case_1();
        break;
    case 2:
        case_2();
        break;
    case 3:
        case_3();
        break;
    case 4:
        case_4();
        break;
    }
    return 0;   
}