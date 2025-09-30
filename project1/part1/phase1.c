#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#define CURRENT_ACCOUNT_AMOUNT 1
struct Account
{
    int account_id;
    double balance;
    int transaction_count;
};

const int TRANSACTIONS_PER_TELLER = 100;
const int NUMBER_OF_THREADS = 1000;
struct Account active_accounts[CURRENT_ACCOUNT_AMOUNT];


void* teller_thread(void* arg) //teller function, which will be sent to each thread as an argument
{
    int teller_id = *(int*) arg;
    unsigned int seed = time(NULL);
    for (int i = 0; i < TRANSACTIONS_PER_TELLER; i++)
    {
        int current_account = rand_r(&seed) % CURRENT_ACCOUNT_AMOUNT; //select a random account
        //if ((rand_r(&seed) % 2) == 0) //deposit or withdraw money into current_account randomly
        if (0 == 0) //deposit only mode
        {
            active_accounts[current_account].balance += 1;
            active_accounts[current_account].transaction_count++;
            printf("Teller %d: Depositing $1.00; account #%d transaction #%d\n", teller_id, current_account, active_accounts[current_account].transaction_count);
        }
        else //withdraw money from current_account
        {
            active_accounts[current_account].balance -= 1;
            active_accounts[current_account].transaction_count++;
            printf("Teller %d: Withdrawing $1.00; account #%d transaction #%d\n", teller_id, current_account, active_accounts[current_account].transaction_count);
        }
    }
    return NULL;
}

int main()
{
    pthread_t threads[NUMBER_OF_THREADS];
    int thread_ids[NUMBER_OF_THREADS];
    for (int i = 0; i < CURRENT_ACCOUNT_AMOUNT; i++)
    {
        active_accounts[i].balance = 10000;
        printf("Initial balance for account %d: $%f\n", i, active_accounts[i].balance);
    }
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, teller_thread, &thread_ids[i]);
    }
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    for(int i = 0; i < CURRENT_ACCOUNT_AMOUNT; i++)
    {
        float expected = 10000 + TRANSACTIONS_PER_TELLER * NUMBER_OF_THREADS;
        printf("Expected final balance for account %d: $%f\n", i, expected);
        printf("Account %d final balance:$%f\n", i, active_accounts[i].balance);
    }
    return 0;
}