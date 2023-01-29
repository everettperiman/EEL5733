#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    // Create duplicate after this point
    fork(); // 1 child
    fork(); // each process gains a child, 3 children 4 process'
    fork(); // each process gains a child 8 process'
    printf("Hello Neso Academy!\n PID = %d\n", getpid());
    return 0;
}