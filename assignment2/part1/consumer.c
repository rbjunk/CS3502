#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int max_lines = -1;
    int verbose = 0;
    int opt;

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
    fprintf (stderr, "Lines: %d\n " , line_count);
    fprintf (stderr, "Character count: %d\n " , character_count);
    return 0;
}