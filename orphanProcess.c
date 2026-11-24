
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        printf("CHILD: My PID = %ld\n", (long)getpid());
        printf("CHILD: Original Parent PID = %ld\n",
               (long)getppid());

        sleep(5);

        printf("CHILD: I am still running!\n");
        printf("CHILD: My new Parent PID = %ld\n",
               (long)getppid());

        return 0;
    }
    else {
        printf("PARENT: My PID = %ld\n", (long)getpid());
        printf("PARENT: My Child PID = %ld\n", (long)pid);

        sleep(2);

        printf("PARENT: I am terminating now!\n");
        return 0;
    }
}
