#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0) { /* Error */
        fprintf(stderr, "Fork Failed");
        return 1;
    }

    if (pid == 0) {
        // Child process
        execlp("/bin/ls", "ls", NULL);
    } else {
        // Parent process
        wait(NULL);
        printf("Child process complete\n");
    }

    return 0;
}