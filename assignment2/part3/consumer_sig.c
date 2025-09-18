#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0;
volatile sig_atomic_t stats_flag = 0;

void handle_sigint ( int sig )
{
    shutdown_flag = 1;
}

void handle_sigusr1 ( int sig )
{
    stats_flag = 1;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = handle_sigint ;
    sigemptyset (& sa.sa_mask ) ;
    sa.sa_flags = 0;
    sigaction (SIGINT , & sa , NULL) ;
    int max_lines = -1;
    int verbose = 0;
    int opt;

    struct sigaction sa_usr1;
    sa_usr1.sa_handler = handle_sigusr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    sigaction(SIGUSR1, &sa_usr1, NULL);

    while ((opt = getopt(argc, argv, "n:v:")) != -1)
    {
        switch (opt)
        {
            case 'n':
            {
                max_lines = atoi(optarg);
                if ((max_lines != -1) && (max_lines < 0))
                {
                    printf("Error, max lines can not be <0\n");
                    return 1;
                }
                break;
            }

            case 'v':
            {
                verbose = atoi(optarg);
                if (verbose < 0)
                {
                    printf("Error, verbose must be true(1) or false(0)\n");
                    return 1;
                }
                break;
            }

            default:
            {
                break;
            }
        }
    }
    char message[4096];
    int line_count = 0;
    int character_count = 0;

    clock_t start = clock();
    while(fgets(message, sizeof(message), stdin) != NULL)
    {
        
        if(verbose == 1)
        {
            printf("%s", message);
            fflush(stdout);
        }
        if ((max_lines != -1) && (line_count >= max_lines))
        {
            break;
        }
        character_count += (strlen(message) - 1);
        line_count++;
    }
    clock_t end = clock();
    double cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC; 
    double throughput = character_count/((1000000)*cpu_time); 
    fprintf (stderr, "Lines: %d\n" , line_count);
    fprintf (stderr, "Character count: %d\n" , character_count);
    fprintf (stderr, "Throughput: %f\n" , throughput);
    while (!shutdown_flag)
    {
        if (stats_flag)
        {
            // Print stats
            fprintf (stderr, "[STATS]\nLines per second: %f\n", (line_count / cpu_time));
            fprintf (stderr, "Bytes per second: %f\n", (character_count / cpu_time));
            fprintf (stderr, "MBytes per second: %f\n", throughput);
            stats_flag = 0;
        }
        sleep(1);
    }
    printf("Gradeful shutdown requested\n shutting down...\n");
    return 0;
}