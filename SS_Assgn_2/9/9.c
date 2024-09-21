/*
============================================================================
Name : 9.c
Author : Ayan Pushp
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - Use signal system call.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("SIGINT ignored.\n");
}

int main() {
    // Ignore SIGINT
    signal(SIGINT, signal_handler);
    
    printf("SIGINT is currently ignored. Press Ctrl+C...\n");
    sleep(5); // Wait for 5 seconds

    // Reset SIGINT to default action
    signal(SIGINT, SIG_DFL);
    printf("SIGINT is now set to default action. Press Ctrl+C...\n");
    
    sleep(5); // Wait for another 5 seconds
    
    printf("Exiting program.\n");
    return 0;
}

