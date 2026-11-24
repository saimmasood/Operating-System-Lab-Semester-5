
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork Failed");
        return 1;
    }
    else if (pid == 0) {
        printf("Child is executing ls command...\n");
        fflush(stdout);

        execlp("/bin/ls", "ls", (char *)NULL);

        perror("exec failed");
        _exit(1);
    }
    else {
        wait(NULL);
        printf("Child Complete\n");
    }

    return 0;
}
