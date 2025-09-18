#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
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
    FILE* input = stdin;
    int buffer_size = 4096;
    int opt;
    char *filename = NULL;
    while ((opt = getopt(argc, argv, "f:b:")) != -1)
    {
        switch (opt)
        {
        case 'f':
        {
            filename = optarg;
            break;
        }

        case 'b':
        {
            buffer_size = atoi(optarg);
            if (buffer_size <= 0)
            {
                printf("Error, buffer size cannot be <= 0\n");
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

    if (filename != NULL)
    {
        input = fopen(filename, "r");
    }

    char *buffer = malloc(buffer_size);
    if (buffer == NULL)
    {
        printf("Error, could not allocate memory for buffer\n");
        return 1;
    }

    size_t bytes_read;
    while((bytes_read = fread(buffer, 1, buffer_size, input)) > 0)
    {
        fwrite(buffer, 1, bytes_read, stdout);
    }

    free(buffer);
    if(input != stdin)
    {
        fclose(input);
    }
    
    return 0;
}