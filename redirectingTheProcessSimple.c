#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <output_file> <command> [arguments]\n",
                argv[0]);
        return 1;
    }

    fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);

    if (fd == -1)
    {
        perror("open failed");
        return 1;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2 failed");
        close(fd);
        return 1;
    }

    close(fd);

    execvp(argv[2], &argv[2]);

    perror("execvp failed");

    return 1;
}