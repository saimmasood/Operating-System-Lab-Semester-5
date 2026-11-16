
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int n = argc - 1;

    if (n <= 0)
    {
        printf("Please enter numbers\n");
        return 1;
    }

    int *arr = malloc(n * sizeof(int));

    if (arr == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        arr[i] = atoi(argv[i + 1]);
    }

    int p1[2], p2[2], p3[2], p4[2];

    pipe(p1);
    pipe(p2);
    pipe(p3);
    pipe(p4);

    pid_t pid1 = fork();

    if (pid1 == 0)
    {
        close(p1[1]);
        close(p2[0]);

        close(p3[0]);
        close(p3[1]);

        close(p4[0]);
        close(p4[1]);

        int *received = malloc(n * sizeof(int));

        read(p1[0], received, n * sizeof(int));

        close(p1[0]);

        int sum = 0;

        for (int i = 0; i < n; i++)
        {
            sum = sum + received[i];
        }

        write(p2[1], &sum, sizeof(sum));

        close(p2[1]);

        free(received);
        return 0;
    }

    else
    {
        pid_t pid2 = fork();

        if (pid2 == 0)
        {
            close(p1[0]);
            close(p1[1]);

            close(p2[0]);
            close(p2[1]);

            close(p3[1]);
            close(p4[0]);

            int *received = malloc(n * sizeof(int));

            read(p3[0], received, n * sizeof(int));

            close(p3[0]);

            int min = received[0];

            for (int i = 1; i < n; i++)
            {
                if (received[i] < min)
                {
                    min = received[i];
                }
            }

            write(p4[1], &min, sizeof(min));

            close(p4[1]);

            free(received);
            return 0;
        }

        else
        {
            close(p1[0]);
            close(p2[1]);

            close(p3[0]);
            close(p4[1]);

            write(p1[1], arr, n * sizeof(int));
            write(p3[1], arr, n * sizeof(int));

            close(p1[1]);
            close(p3[1]);

            int sum, min;

            read(p2[0], &sum, sizeof(sum));
            read(p4[0], &min, sizeof(min));

            printf("Sum = %d\n", sum);
            printf("Minimum = %d\n", min);

            close(p2[0]);
            close(p4[0]);

            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
        }
    }

    free(arr);

    return 0;
}
