#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pids[3];
    int status;
    int i;
    for (i = 0; i < 3; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {
            printf("Child %d (PID: %d) is running.\n", i + 1, getpid());
            sleep(2 + i);  // Sleep for a different time to simulate work
            printf("Child %d (PID: %d) is exiting.\n", i + 1, getpid());
            exit(i + 1);  // Exit with status code corresponding to its number
        }
    }

    printf("Parent process (PID: %d) will now wait for Child 2 (PID: %d).\n", getpid(), pids[1]);
    pid_t wpid = waitpid(pids[1], &status, 0);

    if (wpid == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
        printf("Child 2 (PID: %d) exited with status %d.\n", wpid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Child 2 (PID: %d) terminated by signal %d.\n", wpid, WTERMSIG(status));
    }

    // Wait for remaining child processes to exit
    for (i = 0; i < 3; i++) {
        if (pids[i] != wpid) {
            waitpid(pids[i], NULL, 0);
        }
    }

    printf("Parent process (PID: %d) has waited for all child processes.\n", getpid());

    return 0;
}

