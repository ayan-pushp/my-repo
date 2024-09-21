/*
============================================================================
Name : 8g.c
Author : Ayan Pushp
Description : Write a separate program using signal system call to catch the following signals.
a. SIGPROF (use setitimer system call)
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handle_sigprof(int signum) {
    printf("Caught SIGPROF (Profiling Timer Expired)\n");
}

int main() {
    struct itimerval prof_timer;
    signal(SIGPROF, handle_sigprof);

    prof_timer.it_value.tv_sec = 1; // Start after 1 second
    prof_timer.it_value.tv_usec = 0;
    prof_timer.it_interval.tv_sec = 0; // No periodic timer
    prof_timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_PROF, &prof_timer, NULL);
    printf("Waiting for SIGPROF...\n");
    pause(); // Wait for SIGPROF

    return 0;
}

