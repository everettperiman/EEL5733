#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// First list the accounts and starting balances
// All bank accounts can be assumed to exist and start off
// ... as non negative integers
// There should be an argument to outline how many workers
// ... there should be


// Create a bank account structure
typedef struct bank_account
{
    int balance;
    pthread_mutex_t lock;
} 
bank_account_t;

// Create global pointer for an array of Bank Accounts 
bank_account_t* bank_accounts;


int CreateBankAccounts(bank_account_t* bank_accounts_ptr, char* filename)
{
    FILE *fptr;
    int bank_acct_index;
    int bank_acct_balance;
    int number_of_accounts = 0;

    fptr = fopen(filename, "r");
    if(fptr==NULL) perror("File Open Error");

    while(fscanf(fptr, "%d %d", &bank_acct_index, &bank_acct_balance))
    {
        bank_accounts_ptr = (bank_account_t*)calloc(1, sizeof(bank_account_t));
        bank_accounts_ptr[number_of_accounts].balance = bank_acct_balance;
        //printf("%d\n", bank_accounts_ptr[number_of_accounts].balance);
        number_of_accounts++;
    }
    fclose(fptr);
    return number_of_accounts;
}

void FreeBankAccounts(bank_account_t* bank_accounts_ptr, int number_of_accts)
{
    free(bank_accounts_ptr);
}

int main(int argc, char* argv[])
{
    char* filename;
    int threadcount;
    int num_of_accounts;

    // If there are no extra args return
    if(argc == 1) return 0;

    // Convert args into variables
    threadcount = atoi(argv[1]);
    filename = argv[2];

    printf("%d\n", threadcount);
    printf("%s\n", filename);
    
    num_of_accounts = CreateBankAccounts(bank_accounts, filename);
    for(int i = 0; i < num_of_accounts; i++)
    {
        printf("%d\n", bank_accounts[i].balance);
    }
    FreeBankAccounts(bank_accounts, num_of_accounts);
}
