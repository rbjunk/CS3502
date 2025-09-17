#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pipe1[2]; //parent to child
    int pipe2[2]; //child to parent
    pid_t pid;

    if(pipe(pipe1) == -1)
    {
        printf("Error, could not create pipe1 (parent to child)\n");
    }
    if(pipe(pipe2) == -1)
    {
        printf("Error, could not create pipe2 (child to parent)\n");
    }
    pid = fork();
    int buffer_size = 4096;
    char* buffer = malloc(buffer_size);
    if(pid == 0)
    {
        //child process
        close(pipe1[1]); //close write end of pipe1
        close(pipe2[0]); //close read end of pipe2
        read(pipe1[0], buffer, buffer_size - 1); //read from the parent into a buffer
        printf("Parent message to child: %s\n", buffer); //printout message from parent
        printf("This is the child process.\n");
        char message[buffer_size];
        printf("Enter message to send to the parent: ");
        fgets(message, sizeof(message), stdin);
        printf("\n");
        write(pipe2[1], message, sizeof(message)); //send a message to the parent from the stdin
    }
    else
    {
        //parent process
        close(pipe1[0]); //close read end of pipe1
        close(pipe2[1]); //close write end of pipe2
        printf("This is the parent process.\n");
        char message[buffer_size];
        printf("Enter message to send to the child: ");
        fgets(message, sizeof(message), stdin);
        printf("\n");
        write(pipe1[1], message, sizeof(message)); //send a message to the child from the stdin
        read(pipe2[0], buffer, buffer_size - 1); //read from the child into a buffer
        printf("Child message to parent: %s\n", buffer); //printout message from child
        wait(NULL);
    }
}
