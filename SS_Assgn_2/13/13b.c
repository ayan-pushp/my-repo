/*
============================================================================
Name : 13b.c
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

int main() {
    pid_t target_pid;

    // Enter the PID of the first program here
    printf("Enter the PID of the first program: ");
    scanf("%d", &target_pid);

    // Send SIGSTOP to the first program
    printf("Sending SIGSTOP to process %d...\n", target_pid);
    kill(target_pid, SIGSTOP);

    // Wait for 2 seconds to ensure the first program is stopped
    sleep(2);

    // Send SIGUSR1 to the first program
    printf("Sending SIGUSR1 to process %d...\n", target_pid);
    kill(target_pid, SIGUSR1);

    return 0;
}

