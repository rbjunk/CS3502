#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#define CURRENT_ACCOUNT_AMOUNT 5
struct Account
{
    int account_id;
    double balance;
    int transaction_count;
    pthread_mutex_t lock; //mutex for current account
};

struct Transfer_Args
{
    int from_account_id;
    int to_account_id;
    double amount;
};

const int NUMBER_OF_THREADS = 5;
struct Account active_accounts[CURRENT_ACCOUNT_AMOUNT];


void *transfer(void* arg)
{
    struct Transfer_Args *args = (struct Transfer_Args *)arg;
    int from_account_id = args->from_account_id;
    int to_account_id = args->to_account_id;
    double amount = args->amount;
    printf("Thread %ld: Attempting transfer from account:%d to account:%d\n", pthread_self(), from_account_id, to_account_id);
    pthread_mutex_lock(&active_accounts[from_account_id].lock);
    printf("Locked account:%d\n", from_account_id);
    //simulate processing delay to create deadlock
    usleep(1000);

    printf("Thread %ld: Waiting for account:%d\n", pthread_self(), to_account_id);
    pthread_mutex_lock(&active_accounts[to_account_id].lock);
    printf("Locked account:%d\n", to_account_id);

    active_accounts[from_account_id].balance -= amount;
    active_accounts[to_account_id].balance += amount;

    pthread_mutex_unlock(&active_accounts[from_account_id].lock);
    pthread_mutex_unlock(&active_accounts[to_account_id].lock);
    free(args);
}

int main()
{
    pthread_t threads[NUMBER_OF_THREADS];
    int thread_ids[NUMBER_OF_THREADS];
    for (int i = 0; i < CURRENT_ACCOUNT_AMOUNT; i++)
    {
        pthread_mutex_init(&active_accounts[i].lock, NULL);
        active_accounts[i].balance = 10000;
        active_accounts[i].account_id = i;
        printf("Initial balance for account %d: $%f\n", active_accounts[i].account_id, active_accounts[i].balance);
    }

    for(int i = 0; i < NUMBER_OF_THREADS; i++) //create a circular account transaction loop to ensure a deadlock scenario
    {
        printf("Attempting to transfer $100\n");
        struct Transfer_Args *args = malloc(sizeof(struct Transfer_Args));
        args->from_account_id = active_accounts[i].account_id;
        args->to_account_id = active_accounts[(i + 1) % CURRENT_ACCOUNT_AMOUNT].account_id;
        args->amount = 100;
        pthread_create(&threads[i], NULL, transfer , args);
    }
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i < CURRENT_ACCOUNT_AMOUNT; i++) //get rid of mutexes
    {
        pthread_mutex_destroy(&active_accounts[i].lock);
    }

    for(int i = 0; i < CURRENT_ACCOUNT_AMOUNT; i++)
    {
        printf("Account %d final balance:$%f\n", i, active_accounts[i].balance);
    }
    return 0;
}