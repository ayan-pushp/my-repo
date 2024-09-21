/*
============================================================================
Name : 8d.c
Author : Ayan Pushp
Description : Write a separate program using signal system call to catch the following signals.
a. SIGALRM(use alarm system call)
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigalrm(int signum) {
    printf("Caught SIGALRM (Alarm Signal)\n");
}

int main() {
    signal(SIGALRM, handle_sigalrm);
    
    alarm(1); // Set an alarm for 1 second
    printf("Waiting for SIGALRM...\n");
    pause(); // Wait for the alarm
    
    return 0;
}

