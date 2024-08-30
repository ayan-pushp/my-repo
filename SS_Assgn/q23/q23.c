#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) {
        printf("Child process (PID: %d) exiting...\n", getpid());
        exit(EXIT_SUCCESS);
    } else {
        printf("Parent process (PID: %d) sleeping...\n", getpid());
        sleep(30);
        printf("Parent process waking up and exiting...\n");
        wait(NULL);
    }
    
    return 0;
}

