#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct
{
    pthread_t* threads;
    int count;
}
thread_collection_t;

thread_collection_t thread_collector;

int main(int argc, char* argv[])
{
    int threadcount = atoi(argv[1]);
    printf("# of Threads is %d\n", threadcount);


    thread_collector.count = threadcount;
    printf("# of Threads is %d\n", thread_collector.count);
    thread_collector.threads = (pthread_t*)malloc(thread_collector.count * sizeof(pthread_t));


    return 1;
}