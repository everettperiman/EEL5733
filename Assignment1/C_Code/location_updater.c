#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int current_pid = getpid();
    int child_ID = 0;
    int pfd[2];
    char *args[] = {NULL};
    setbuf(stdout, NULL);

    // Need to create 3 pipes?
    // One pipe from parent STDIN to email STDIN
    // One pipe from email STDOUT to calendar STDIN
    // One pipe from calendar STDOUT to parent STDOUT
    
    pipe(pfd);

    if(current_pid != 0)
    {
        // At this point there will be two process' running the same code
        current_pid = fork();
    }

    // If it is the child process start the email filter
    if(current_pid == 0 && child_ID == 0)
    {
        // Currently in the email filter program area
        printf("Child ID is %d\n", child_ID);
        //dup2(pfd[0], STDOUT_FILENO);
        //close(pfd[1]);
        execv("./email_filter", args);
    }  
    else
    {
        // Create a tunnel from LocationUpdater STDOUT to pfd[0]
        //dup2(pfd[1], 0);
        child_ID = 1;
        current_pid = fork();
    }

    if(current_pid == 0 && child_ID ==1)
    {
        // Currently in the calendar filter program area
        //dup2(pfd[1], STDIN_FILENO);
        //close(pfd[0]);
        printf("Child ID is %d\n", child_ID);
        execv("./calendar_filter", args);
    }

    wait(NULL);

}