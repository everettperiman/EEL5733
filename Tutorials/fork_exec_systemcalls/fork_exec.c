#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int x;
    printf("We are in the parent process %d\n", getpid());
    x = fork();
    if(x == 0)
    {
        printf("We are in the child process\n");
        char *args[] = {"Hello", "Neso", "Academy", NULL};
        execv("./exec_2.out", args);
    }
    else
    {
        printf("We are in the parent proces\n");
    }
    return 0;
}