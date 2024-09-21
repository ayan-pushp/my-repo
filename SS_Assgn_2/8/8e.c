/*
============================================================================
Name : 8e.c
Author : Ayan Pushp
Description : Write a separate program using signal system call to catch the following signals.
a. SIGALRM (use setitimer system call)
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handle_sigalrm(int signum) {
    printf("Caught SIGALRM (Alarm Signal using setitimer)\n");
}

int main() {
    struct itimerval timer;
    signal(SIGALRM, handle_sigalrm);

    timer.it_value.tv_sec = 1; // Start after 1 second
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0; // No periodic timer
    timer.it_interval.tv_usec = 0;
    
    setitimer(ITIMER_REAL, &timer, NULL);
    printf("Waiting for SIGALRM...\n");
    pause(); // Wait for the alarm
    
    return 0;
}

