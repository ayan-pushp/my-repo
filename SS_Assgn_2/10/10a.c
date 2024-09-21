/*
============================================================================
Name : 10a.c
Author : Ayan Pushp
Description : Write a separate program using sigaction system call to catch the following signals.
a. SIGSEGV
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf jump_buffer;

void handle_sigsegv(int signum) {
    printf("Caught SIGSEGV (Segmentation Fault)\n");
    longjmp(jump_buffer, 1);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_sigsegv;
    sa.sa_flags = 0; // No special flags
    sigemptyset(&sa.sa_mask); // No additional signals blocked

    sigaction(SIGSEGV, &sa, NULL);

    if (setjmp(jump_buffer) == 0) {
        // Trigger SIGSEGV
        printf("Triggering SIGSEGV...\n");
        int *p = NULL; // Dereferencing NULL to cause a segmentation fault
        *p = 1; // This will cause SIGSEGV
    }

    printf("Exiting program.\n");
    return 0;
}

