#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int p1[2], p2[2], p3[2], p4[2];
    int arr[100], n, i;
    int sum = 0, min;

    pid_t pid1, pid2;

    n = argc - 1;

    for (i = 0; i < n; i++)
    {
        arr[i] = atoi(argv[i + 1]);
    }

    pipe(p1);
    pipe(p2);
    pipe(p3);
    pipe(p4);

    pid1 = fork();

    if (pid1 == 0)
    {
        close(p1[1]);
        close(p2[0]);
        close(p3[0]);
        close(p3[1]);
        close(p4[0]);
        close(p4[1]);

        read(p1[0], arr, n * sizeof(int));

        for (i = 0; i < n; i++)
        {
            sum = sum + arr[i];
        }

        write(p2[1], &sum, sizeof(int));

        close(p1[0]);
        close(p2[1]);
    }
    else
    {
        pid2 = fork();

        if (pid2 == 0)
        {
            close(p1[0]);
            close(p1[1]);
            close(p2[0]);
            close(p2[1]);
            close(p3[1]);
            close(p4[0]);

            read(p3[0], arr, n * sizeof(int));

            min = arr[0];

            for (i = 1; i < n; i++)
            {
                if (arr[i] < min)
                {
                    min = arr[i];
                }
            }

            write(p4[1], &min, sizeof(int));

            close(p3[0]);
            close(p4[1]);
        }
        else
        {
            close(p1[0]);
            close(p2[1]);
            close(p3[0]);
            close(p4[1]);

            write(p1[1], arr, n * sizeof(int));
            write(p3[1], arr, n * sizeof(int));

            read(p2[0], &sum, sizeof(int));
            read(p4[0], &min, sizeof(int));

            printf("Sum = %d\n", sum);
            printf("Minimum = %d\n", min);

            close(p1[1]);
            close(p2[0]);
            close(p3[1]);
            close(p4[0]);

            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
        }
    }

    return 0;
}