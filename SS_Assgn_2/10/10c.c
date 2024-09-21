/*
============================================================================
Name : 10c.c
Author : Ayan Pushp
Description : Write a separate program using sigaction system call to catch the following signals.
a. SIGFPE
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handle_sigfpe(int signum) {
    printf("Caught SIGFPE (Floating Point Exception)\n");
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_sigfpe;
    sa.sa_flags = 0; // No special flags
    sigemptyset(&sa.sa_mask); // No additional signals blocked

    sigaction(SIGFPE, &sa, NULL);

    // Trigger SIGFPE
    printf("Triggering SIGFPE...\n");
    int x = 1;
    int y = 0;
    printf("Result: %d\n", x / y); // This will cause SIGFPE

    return 0;
}

