#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        printf("Child process.\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID of child: %d\n", getppid());
    } else {
        printf("Parent process.\n");
        printf("Parent PID: %d\n", getpid());
        printf("Child PID created by parent: %d\n", pid);
    }

    return 0;
}

