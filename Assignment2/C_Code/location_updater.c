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
    
    // Create a forked child process
    current_pid = fork();

    // If it is the child process start the email filter
    if(current_pid == 0)
    {
        // Only the children share a pipe
        pipe(pfd);
        current_pid = fork();
        if(current_pid == 0)
        {
            // This would be the grand child
            dup2(pfd[0], STDIN_FILENO);
            close(pfd[1]);
            execv("./calendar_filter", args);
        }
        else
        {
            // This would be the first child
            dup2(pfd[1], STDOUT_FILENO);
            close(pfd[0]);
            execv("./email_filter", args);
        }
    }   

    wait(NULL);
}