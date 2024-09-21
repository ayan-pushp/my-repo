/*
============================================================================
Name : 8c.c
Author : Ayan Pushp
Description : Write a separate program using signal system call to catch the following signals.
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
    signal(SIGFPE, handle_sigfpe);
    
    // Trigger SIGFPE
    printf("Triggering SIGFPE...\n");
    int x = 1;
    int y = 0;
    printf("Result: %d\n", x / y); // This will cause SIGFPE
    
    return 0;
}

