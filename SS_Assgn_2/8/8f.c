/*
============================================================================
Name : 8f.c
Author : Ayan Pushp
Description : Write a separate program using signal system call to catch the following signals.
a. SIGVTALRM (use setitimer system call)
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handle_sigvtalrm(int signum) {
    printf("Caught SIGVTALRM (Virtual Timer Expired)\n");
}

int main() {
    struct itimerval vt_timer;
    signal(SIGVTALRM, handle_sigvtalrm);

    vt_timer.it_value.tv_sec = 1; // Start after 1 second
    vt_timer.it_value.tv_usec = 0;
    vt_timer.it_interval.tv_sec = 0; // No periodic timer
    vt_timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_VIRTUAL, &vt_timer, NULL);
    printf("Waiting for SIGVTALRM...\n");
    pause(); // Wait for SIGVTALRM

    return 0;
}

