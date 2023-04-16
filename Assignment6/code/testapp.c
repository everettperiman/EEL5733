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
    char* filename = "/dev/a5";
    fd_a = open(filename, O_RDWR);
    if (fd_a == -1) {
        printf("Error: could not open file %s\n", filename);
        return 1;
    }
    printf("Open fd_a\n");
    fd_b = open(filename, O_RDWR);
    if (fd_b == -1) {
        printf("Error: could not open file %s\n", filename);
        return 1;
    }
    printf("Open fd_b\n");
    close(fd_a);
    close(fd_b);
    return 0;   
}