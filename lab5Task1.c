// implimenting this command , pipelines in linux
//ls / -l > input.txt

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int fd;

    pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        fd = open("input.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);

        if (fd == -1)
        {
            perror("open failed");
            exit(1);
        }

        dup2(fd, STDOUT_FILENO);

        close(fd);

        char *args[] = {"ls", "/", "-l", NULL};

        execvp("ls", args);

        perror("exec failed");
        exit(1);
    }
    else
    {
        wait(NULL);
        printf("Command completed.\n");
    }

    return 0;
}