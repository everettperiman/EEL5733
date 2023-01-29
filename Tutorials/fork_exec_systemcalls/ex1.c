#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("PID of ex1.c = %d\n", getpid());
    // Args must be null terminated
    char *args[] = {"Hello", "Neso", "Academy", NULL};
    execv("./exec_2.out", args);
    printf("Back to ex1.c");

    return 0;
}