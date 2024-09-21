/*
============================================================================
Name : 1a.c
Author : Ayan Pushp
Description : Write a separate program (for each time domain) to set a interval timer in 10 sec and 10 micro second
a. ITIMER_PROF
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

void timer_handler(int signum) {
    printf("ITIMER_PROF timer expired!\n");
    exit(0);
}

int main() {
    struct itimerval timer;

    // Set up the signal handler
    signal(SIGPROF, timer_handler);

    // Configure timer for 10 seconds and 10 microseconds
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0; // one-time timer

    // Start the timer
    setitimer(ITIMER_PROF, &timer, NULL);
    
    while (1) {
        // Perform some busy work to use CPU time
        for (volatile int i = 0; i < 100000000; i++) {
            // No operation, just consuming CPU cycles
        }

        // Optional: break after one iteration for testing purposes
        //break; // Remove this line if you want it to run indefinitely
    }

    // Wait for timer to expire
    while (1) {
        pause(); // suspend process until signal is caught
    }

    return 0;
}
