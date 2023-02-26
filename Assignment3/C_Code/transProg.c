#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_NUM_ACCOUNTS 10000
#define MAX_NUM_TRANSFERS 100000
#define MAX_STR_LEN 1000

// First list the accounts and starting balances
// All bank accounts can be assumed to exist and start off
// ... as non negative integers
// There should be an argument to outline how many workers
// ... there should be

// Create a structure to store threads
typedef struct
{
    pthread_t* threads;
    int count;
}
thread_collection_t;


// Create a bank account structure
typedef struct
{
    int balance;
    pthread_mutex_t lock;
} 
bank_account_t;

// Create a bank transfer statment structure
typedef struct
{
    int account_from;
    int account_to;
    int transfer_amount;
} 
bank_transfer_t;


// Create global pointer for an array of Bank Accounts 
thread_collection_t thread_collector;
static pthread_mutex_t transfer_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t open_transfer = PTHREAD_COND_INITIALIZER;
bank_transfer_t transfer_statments[MAX_NUM_TRANSFERS];
bank_account_t bank_accounts[MAX_NUM_ACCOUNTS];
int number_of_accounts, number_of_transfers, transfer_status = 0;



int ProcessTransfer(bank_transfer_t transfer_statment)
{ 
    bank_accounts[transfer_statment.account_from].balance = bank_accounts[transfer_statment.account_from].balance 
                                                                                - transfer_statment.transfer_amount;
    bank_accounts[transfer_statment.account_to].balance = bank_accounts[transfer_statment.account_to].balance 
                                                                                + transfer_statment.transfer_amount;
}


int CreateAccounts(char* filename)
{
    FILE *fptr;
    char line_buffer[100];
    int bank_acct_index, bank_acct_balance;
    int account_from, account_to, transfer_amount;
    number_of_accounts = 0, number_of_transfers = 0;

    fptr = fopen(filename, "r");
    if(fptr==NULL) perror("File Open Error");
    
    while(fgets(line_buffer, sizeof(line_buffer), fptr) != NULL)
    {
        if(sscanf(line_buffer, "%d %d", &bank_acct_index, &bank_acct_balance) == 2)
        {
            bank_accounts[number_of_accounts].balance = bank_acct_balance;
            pthread_mutex_init(&bank_accounts[number_of_accounts].lock, NULL);
            number_of_accounts++;
        }
        else if(sscanf(line_buffer, "%*s %d %d %d", &account_from, &account_to, &transfer_amount) == 3)
        {
            transfer_statments[number_of_transfers].account_from = account_from-1;
            transfer_statments[number_of_transfers].account_to = account_to-1;
            transfer_statments[number_of_transfers].transfer_amount = transfer_amount;
            number_of_transfers++;
        }
    }
    fclose(fptr);
    return 1;
}

void* worker_thread(void* args)
{
    // Two variables to track what the thread should work on and if it completed its work
    int assigned_transfer = 0;
    int job_completed = 1;
    int tid = pthread_self();
    int jobs = 0;
    while(1)
    {
        // This section is the critical region for the shared job buffer
        pthread_mutex_lock(&transfer_lock);
        if(transfer_status == number_of_transfers)
        {
            printf("Thread %d completed %d jobs\n", tid, jobs);
            pthread_mutex_unlock(&transfer_lock);
            return NULL;
        }
        else
        {
            jobs++;
            job_completed = 0;
            assigned_transfer = transfer_status;
            transfer_status++;
            pthread_mutex_unlock(&transfer_lock);
        }

        // Once the job has been assigned it is time to do the work
        while(job_completed == 0)
        {
            // Check to see if the from bank account is in use
            // If it is not then lock this account
            if(pthread_mutex_trylock(&bank_accounts[transfer_statments[assigned_transfer].account_from].lock) == 0)
            {
                if(pthread_mutex_trylock(&bank_accounts[transfer_statments[assigned_transfer].account_to].lock) == 0)
                {
                    // We now own the lock on both accounts
                    ProcessTransfer(transfer_statments[assigned_transfer]);
                    job_completed = 1;
                    pthread_mutex_unlock(&bank_accounts[transfer_statments[assigned_transfer].account_to].lock);
                    pthread_mutex_unlock(&bank_accounts[transfer_statments[assigned_transfer].account_from].lock);
                }
                else
                {
                    // Release the first lock and try again
                    pthread_mutex_unlock(&bank_accounts[transfer_statments[assigned_transfer].account_from].lock);
                }
            }
        }
    }
}


int main(int argc, char* argv[])
{
    char* filename;
    int threadcount;

    // If there are no extra args return
    if(argc == 1) return 0;

    // Convert args into variables
    thread_collector.count = atoi(argv[1]);
    filename = argv[2];
    
    thread_collector.threads = (pthread_t*)malloc(thread_collector.count * sizeof(pthread_t));

    // Debug strings
    //printf("%d\n", thread_collector.count);
    //printf("%s\n", filename);
    
    // Create the accounts and transfers
    CreateAccounts(filename);

    // Create the threads
    for(int i = 0; i < thread_collector.count; i++)
    {
        pthread_create(&thread_collector.threads[i], NULL, &worker_thread, NULL);
    }

    for(int i = 0; i < thread_collector.count; i++)
    {
        pthread_join(thread_collector.threads[i], NULL);
    }

    // Print the final status of each account
    for(int i = number_of_accounts-5; i < number_of_accounts; i++)
    {
        printf("%d %d\n", i + 1, bank_accounts[i].balance);
    }

    // Exit the program
    return 1;
}
