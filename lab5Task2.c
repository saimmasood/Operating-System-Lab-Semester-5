//implimenting this command ,linux pipeline IPC
//grep "m" < input.txt | sort > output.txt



#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    int input, output;
    pid_t pid1, pid2;

    input = open("input.txt", O_RDONLY);

    if (input == -1)
    {
        perror("input file failed");
        return 1;
    }

    output = open("output.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);

    if (output == -1)
    {
        perror("output file failed");
        close(input);
        return 1;
    }

    if (pipe(fd) == -1)
    {
        perror("pipe failed");
        close(input);
        close(output);
        return 1;
    }

    pid1 = fork();

    if (pid1 == -1)
    {
        perror("fork failed");
        return 1;
    }

    if (pid1 == 0)
    {
        dup2(input, STDIN_FILENO);
        dup2(fd[1], STDOUT_FILENO);

        close(input);
        close(output);
        close(fd[0]);
        close(fd[1]);

        char *args[] = {"grep", "m", NULL};

        execvp("grep", args);

        perror("grep failed");
        exit(1);
    }

    pid2 = fork();

    if (pid2 == -1)
    {
        perror("fork failed");
        return 1;
    }

    if (pid2 == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        dup2(output, STDOUT_FILENO);

        close(input);
        close(output);
        close(fd[0]);
        close(fd[1]);

        char *args[] = {"sort", NULL};

        execvp("sort", args);

        perror("sort failed");
        exit(1);
    }

    close(input);
    close(output);
    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    printf("Pipeline completed.\n");

    return 0;
}