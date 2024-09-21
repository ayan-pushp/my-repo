/*
============================================================================
Name : 11.c
Author : Ayan Pushp
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal. Use sigaction system call.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("SIGINT ignored.\n");
}

int main() {
    struct sigaction sa;

    // Step 1: Ignore SIGINT
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0; // No special flags
    sigemptyset(&sa.sa_mask); // No additional signals blocked

    sigaction(SIGINT, &sa, NULL);

    printf("SIGINT is currently ignored. Press Ctrl+C...\n");
    sleep(5); // Wait for 5 seconds

    // Step 2: Reset SIGINT to default action
    sa.sa_handler = SIG_DFL; // Reset to default
    sigaction(SIGINT, &sa, NULL);

    printf("SIGINT is now set to default action. Press Ctrl+C...\n");
    sleep(5); // Wait for another 5 seconds

    printf("Exiting program.\n");
    return 0;
}

