/*
============================================================================
Name : 3.c
Author : Ayan Pushp
Description : Write a program to set (any one) system resource limit. Use setrlimit system call.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    struct rlimit limit;

    // Print the current limit
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }
    printf("Current limit for open files:\n");
    printf("  Soft Limit: %lu\n", limit.rlim_cur);
    printf("  Hard Limit: %lu\n\n", limit.rlim_max);

    // Set a new soft limit for open files
    limit.rlim_cur = 2048; // Set new soft limit to 1024
    

    // Apply the new limit
    if (setrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("setrlimit");
        exit(EXIT_FAILURE);
    }

    // Print the new limit
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }
    printf("New limit for open files:\n");
    printf("  Soft Limit: %lu\n", limit.rlim_cur);
    printf("  Hard Limit: %lu\n", limit.rlim_max);

    return 0;
}

