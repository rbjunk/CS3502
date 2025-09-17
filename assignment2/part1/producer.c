#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
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