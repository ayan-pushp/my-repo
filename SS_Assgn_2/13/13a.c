/*
============================================================================
Name : 13a.c
Author : Ayan Pushp
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program will send the signal (using kill system call). Find out whether the first program is able to catch
the signal or not.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigusr1(int signum) {
    printf("Caught SIGUSR1 signal.\n");
}

int main() {
    struct sigaction sa;

    // Setup the handler for SIGUSR1
    sa.sa_handler = handle_sigusr1;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);

    printf("Waiting for SIGUSR1 signal. My PID is %d.\n", getpid());

    // Infinite loop waiting for signals
    while (1) {
        pause(); // Wait for any signal
    }

    return 0;
}

