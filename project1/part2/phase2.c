#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#define CURRENT_ACCOUNT_AMOUNT 1 //static int for total number of accounts
struct Account //structure for the account data
{
    int account_id;
    double balance;
    int transaction_count;
    pthread_mutex_t lock; //mutex for current account
};

char mode = 'd'; //global variable for transaction mode
const int TRANSACTIONS_PER_TELLER = 10000; //the number of deposit or withdraw transactions each teller will perform
const int NUMBER_OF_THREADS = 100; //the number of threads (amount of tellers)
struct Account active_accounts[CURRENT_ACCOUNT_AMOUNT]; //create accounts

void* teller_thread(void* arg) //teller function, which will be sent to each thread as an argument
{
    int teller_id = *(int*) arg; //cast teller_id from arg pointer
    unsigned int seed = time(NULL); //initialize a (somewhat) unique seed for this thread
    for (int i = 0; i < TRANSACTIONS_PER_TELLER; i++)
    {
        int current_account = rand_r(&seed) % CURRENT_ACCOUNT_AMOUNT; //select a random account
        //if ((rand_r(&seed) % 2) == 0) //deposit or withdraw money into current_account randomly
        if (mode == 'd') //deposit only mode
        {
            pthread_mutex_lock(&active_accounts[current_account].lock);
            active_accounts[current_account].balance += 1;
            active_accounts[current_account].transaction_count++;
            //printf("Teller %d: Depositing $1.00; account #%d transaction #%d\n", teller_id, current_account, active_accounts[current_account].transaction_count);
            pthread_mutex_unlock(&active_accounts[current_account].lock);
        }
        else //withdraw money from current_account
        {
            pthread_mutex_lock(&active_accounts[current_account].lock);
            active_accounts[current_account].balance -= 1;
            active_accounts[current_account].transaction_count++;
            //printf("Teller %d: Withdrawing $1.00; account #%d transaction #%d\n", teller_id, current_account, active_accounts[current_account].transaction_count);
            pthread_mutex_unlock(&active_accounts[current_account].lock);
        }
    }
    if (mode == 'd')
    {
        printf("Teller %d: performed %d deposits\n", teller_id, TRANSACTIONS_PER_TELLER);
    }
    else
    {
        printf("Teller %d: performed %d deposits\n", teller_id, TRANSACTIONS_PER_TELLER);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "dwh")) != -1)
    {
        switch(opt)
        {
            case 'd':
            {
                mode = 'd';
                break;
            }
            case 'w':
            {
                mode = 'w';
                break;
            }
            case 'h':
            default:
            {
                printf("Usage: [-d deposit mode (default)] [-w withdraw mode]\n");
                return 1;
            }
        }
    }

    pthread_t threads[NUMBER_OF_THREADS]; //create array of threads
    int thread_ids[NUMBER_OF_THREADS]; //create array of thread ids
    for (int i = 0; i < CURRENT_ACCOUNT_AMOUNT; i++) //initialize the accounts with a starting balance and initialize a mutex
    {
        pthread_mutex_init(&active_accounts[i].lock, NULL);
        active_accounts[i].balance = 10000;
        active_accounts[i].account_id = i;
        printf("Initial balance for account %d: $%f\n", active_accounts[i].account_id, active_accounts[i].balance);
    }
    double start = clock(); //performance clock
    for (int i = 0; i < NUMBER_OF_THREADS; i++) //create the threads and assign them to the task
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, teller_thread, &thread_ids[i]);
    }
    for (int i = 0; i < NUMBER_OF_THREADS; i++) //wait until all threads are finished
    {
        pthread_join(threads[i], NULL);
    }
    double end = clock();
    double total_cpu_time = (end - start) / CLOCKS_PER_SEC; //calculate total time (in seconds) that the tasks took
    for(int i = 0; i < CURRENT_ACCOUNT_AMOUNT; i++) //get rid of mutexes
    {
        pthread_mutex_destroy(&active_accounts[i].lock);
    }
    for(int i = 0; i < CURRENT_ACCOUNT_AMOUNT; i++) //calculate the expected final balance and show the actual final balance. Also show performance stats
    {
        float expected;
        if (mode == 'd')
        {
            expected = 10000 + TRANSACTIONS_PER_TELLER * NUMBER_OF_THREADS;
        }
        else
        {
            expected = 10000 - TRANSACTIONS_PER_TELLER * NUMBER_OF_THREADS;
        }
        printf("Account %d: expected final balance:$%f\n", i, expected);
        printf("Account %d: final balance:$%f\n", active_accounts[i].account_id, active_accounts[i].balance);
        printf("total cpu time: %f\n", total_cpu_time);
    }
    return 0;
}