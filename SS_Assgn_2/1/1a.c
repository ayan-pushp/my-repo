/*
============================================================================
Name : 1a.c
Author : Ayan Pushp
Description : Write a separate program (for each time domain) to set a interval timer in 10 sec and 10 micro second
a. ITIMER_REAL
Date: 20th Sep, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

void timer_handler(int signum) {
    printf("ITIMER_REAL timer expired!\n");
}

int main() {
    struct itimerval timer;

    // Set up the signal handler
    signal(SIGALRM, timer_handler);

    // Configure timer for 10 seconds and 10 microseconds
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0; // one-time timer

    // Start the timer
    setitimer(ITIMER_REAL, &timer, NULL);

    // Wait for timer to expire
    while (1) {
        pause(); // suspend process until signal is caught
    }

    return 0;
}

