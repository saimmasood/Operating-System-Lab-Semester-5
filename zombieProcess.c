
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        printf("CHILD: My PID = %ld\n", (long)getpid());
        printf("CHILD: I am terminating now!\n");
        fflush(stdout);

        _exit(0);
    }
    else {
        printf("PARENT: My PID = %ld\n", (long)getpid());
        printf("PARENT: Child PID = %ld\n", (long)pid);

        printf("PARENT: Sleeping for 30 seconds...\n");
        fflush(stdout);

        // Child exits but parent does not reap it yet
        sleep(30);

        // Parent finally collects child's status
        waitpid(pid, NULL, 0);

        printf("PARENT: Child reaped. Zombie removed.\n");
    }

    return 0;
}
