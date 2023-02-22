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
static pthread_mutex_t transfer_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t open_transfer = PTHREAD_COND_INITIALIZER;
bank_transfer_t transfer_statments[MAX_NUM_TRANSFERS];
bank_account_t bank_accounts[MAX_NUM_ACCOUNTS];
int number_of_accounts, number_of_transfers, transfer_status;



int ProcessTransfer(bank_transfer_t transfer_statment)
{
    bank_accounts[transfer_statment.account_from].balance = bank_accounts[transfer_statment.account_from].balance 
                                                                                - transfer_statment.transfer_amount;
    bank_accounts[transfer_statment.account_to].balance = bank_accounts[transfer_statment.account_to].balance 
                                                                                + transfer_statment.transfer_amount;
}


int CreateBankAccounts(char* filename)
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

int main(int argc, char* argv[])
{
    char* filename;
    int threadcount;

    // If there are no extra args return
    if(argc == 1) return 0;

    // Convert args into variables
    threadcount = atoi(argv[1]);
    filename = argv[2];

    printf("%d\n", threadcount);
    printf("%s\n", filename);
    
    // Create the accounts and transfers
    CreateBankAccounts(filename);

    // Assign the transfers to each thread
    for(int i = 0; i < number_of_transfers; i++)
    {
        ProcessTransfer(transfer_statments[i]);
    }

    // Print the final status of each account
    for(int i = 0; i < number_of_accounts; i++)
    {
        printf("%d %d\n", i + 1, bank_accounts[i].balance);
    }

    // Exit the program
    return 1;
}
