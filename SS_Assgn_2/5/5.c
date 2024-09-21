/*
============================================================================
Name : 5.c
Author : Ayan Pushp
Description : Write a program to print the system limitation of
a. maximum length of the arguments to the exec family of functions.
b. maximum number of simultaneous process per user id.
c. number of clock ticks (jiffy) per second.
d. maximum number of open files
e. size of a page
f. total number of pages in the physical memory
g. number of currently available pages in the physical memory
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <limits.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    struct rlimit limit;
    long pagesize;
    struct sysinfo sys_info;

    // a. Maximum length of the arguments to the exec family of functions
    long arg_max = sysconf(_SC_ARG_MAX);
    printf("Maximum length of arguments to exec functions: %ld bytes\n", arg_max);

    // b. Maximum number of simultaneous processes per user ID
    if (getrlimit(RLIMIT_NPROC, &limit) == -1) {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }
    printf("Maximum number of simultaneous processes per user ID: Soft Limit: %lu, Hard Limit: %lu\n", limit.rlim_cur, limit.rlim_max);

    // c. Number of clock ticks (jiffy) per second
    printf("Number of clock ticks (jiffy) per second: %ld\n", sysconf(_SC_CLK_TCK));

    // d. Maximum number of open files
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }
    printf("Maximum number of open files: Soft Limit: %lu, Hard Limit: %lu\n", limit.rlim_cur, limit.rlim_max);

    // e. Size of a page
    pagesize = sysconf(_SC_PAGESIZE);
    printf("Size of a page: %ld bytes\n", pagesize);

    // f. Total number of pages in the physical memory
    if (sysinfo(&sys_info) == -1) {
        perror("sysinfo");
        exit(EXIT_FAILURE);
    }
    printf("Total number of pages in the physical memory: %ld\n", sys_info.totalram / pagesize);

    // g. Number of currently available pages in the physical memory
    printf("Number of currently available pages in the physical memory: %ld\n", sys_info.freeram / pagesize);

    return 0;
}

