/*
============================================================================
Name : 12.c
Author : Ayan Pushp
Description : Write a program to create an orphan process. Use kill system call to send SIGKILL signal to the parent process from the child process.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork(); // Create a new process

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid > 0) {
        // Parent process
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
        
        // Sleep to ensure the child runs first and sends SIGKILL
        sleep(2);
        printf("Parent process exiting...\n");
        exit(0); // Parent process exits
    } else {
        // Child process
        sleep(1); // Wait a bit to ensure the parent is still running
        printf("Child process (PID: %d) sending SIGKILL to parent (PID: %d)\n", getpid(), getppid());

        // Send SIGKILL to the parent process
        kill(getppid(), SIGKILL);

        // After sending the signal, the child becomes an orphan
        printf("Child process now orphaned. New parent will be init (PID 1).\n");

        // Sleep to allow observation of the orphan state
        sleep(5);
    }

    return 0;
}

