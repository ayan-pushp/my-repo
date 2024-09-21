/*
============================================================================
Name : 8b.c
Author : Ayan Pushp
Description : Write a separate program using signal system call to catch the following signals.
a. SIGINT
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigint(int signum) {
    printf("Caught SIGINT (Interrupt Signal)\n");
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);
    
    printf("Press Ctrl+C to trigger SIGINT...\n");
    while (1) {
        pause(); // Wait for signals
    }
    
    return 0;
}

