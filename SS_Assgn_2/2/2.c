/*
============================================================================
Name : 2.c
Author : Ayan Pushp
Description : Write a program to print the system resource limits. Use getrlimit system call.
Date: 20th Sep, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void print_resource_limit(int resource, const char *name) {
    struct rlimit limit;

    // Get the resource limit
    if (getrlimit(resource, &limit) == -1) {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }

    // Print the resource limits
    printf("Resource: %s\n", name);
    printf("  Soft Limit: %lu\n", limit.rlim_cur);
    printf("  Hard Limit: %lu\n\n", limit.rlim_max);
}

int main() {
    // Print limits for common resources
    print_resource_limit(RLIMIT_AS, "Address Space Limit");
    print_resource_limit(RLIMIT_CORE, "Core File Size Limit");
    print_resource_limit(RLIMIT_CPU, "CPU Time Limit");
    print_resource_limit(RLIMIT_DATA, "Data Segment Size Limit");
    print_resource_limit(RLIMIT_FSIZE, "File Size Limit");
    print_resource_limit(RLIMIT_NOFILE, "Number of Open Files");
    print_resource_limit(RLIMIT_STACK, "Stack Size Limit");
    print_resource_limit(RLIMIT_MEMLOCK, "Maximum Locked-in-Memory Address Space");
    print_resource_limit(RLIMIT_NPROC, "Number of Processes");

    return 0;
}

