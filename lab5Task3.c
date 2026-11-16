//implimenting this command , Linux pipelines , IPC
// ls / -lR 2> error.txt | grep "Nov" | sort | less


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int fd[3][2];
    int error_fd;
    pid_t pid[4];

    error_fd = open("error.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);

    if (error_fd == -1)
    {
        perror("error file failed");
        return 1;
    }

    for (int i = 0; i < 3; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("pipe failed");
            return 1;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        pid[i] = fork();

        if (pid[i] == -1)
        {
            perror("fork failed");
            return 1;
        }

        if (pid[i] == 0)
        {
            if (i > 0)
            {
                dup2(fd[i - 1][0], STDIN_FILENO);
            }

            if (i < 3)
            {
                dup2(fd[i][1], STDOUT_FILENO);
            }

            if (i == 0)
            {
                dup2(error_fd, STDERR_FILENO);
            }

            for (int j = 0; j < 3; j++)
            {
                close(fd[j][0]);
                close(fd[j][1]);
            }

            close(error_fd);

            if (i == 0)
            {
                char *args[] = {"ls", "/", "-lR", NULL};
                execvp("ls", args);
            }
            else if (i == 1)
            {
                char *args[] = {"grep", "Nov", NULL};
                execvp("grep", args);
            }
            else if (i == 2)
            {
                char *args[] = {"sort", NULL};
                execvp("sort", args);
            }
            else
            {
                char *args[] = {"less", NULL};
                execvp("less", args);
            }

            perror("exec failed");
            exit(1);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        close(fd[i][0]);
        close(fd[i][1]);
    }

    close(error_fd);

    for (int i = 0; i < 4; i++)
    {
        waitpid(pid[i], NULL, 0);
    }

    return 0;
}