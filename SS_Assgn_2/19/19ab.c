/*
============================================================================
Name : 19ab.c
Author : Ayan Pushp
Description : Create a FIFO file by
a. mknod command
b. mkfifo command
use strace command to find out, which command (mknod or mkfifo) is better.

Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Command to trace mknod
    const char *mknod_command = "strace -o mknod_trace.txt mknod my_fifo p";
    
    // Command to trace mkfifo
    const char *mkfifo_command = "strace -o mkfifo_trace.txt mkfifo my_fifo";

    // Execute the mknod command with strace
    printf("Running: %s\n", mknod_command);
    int mknod_result = system(mknod_command);
    if (mknod_result == -1) {
        perror("Error executing mknod command");
        return EXIT_FAILURE;
    }
    printf("mknod command executed. Trace saved to mknod_trace.txt\n");

    // Execute the mkfifo command with strace
    printf("Running: %s\n", mkfifo_command);
    int mkfifo_result = system(mkfifo_command);
    if (mkfifo_result == -1) {
        perror("Error executing mkfifo command");
        return EXIT_FAILURE;
    }
    printf("mkfifo command executed. Trace saved to mkfifo_trace.txt\n");

    return EXIT_SUCCESS;
}

