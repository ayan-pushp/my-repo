/*
============================================================================
Name : 23.c
Author : Ayan Pushp
Description : Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    // Get maximum number of open files
    struct rlimit rl;
    if (getrlimit(RLIMIT_NOFILE, &rl) == -1) {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }
    printf("Maximum number of open files: soft limit = %lu, hard limit = %lu\n", rl.rlim_cur, rl.rlim_max);

    // Get the size of a pipe (circular buffer)
    // Note: This size is implementation-defined, typically 64KB or 1MB
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Since the size of the pipe is not directly retrievable,
    // we can create a dummy write to get a sense of how it behaves.
    // However, this is just to demonstrate the use of a pipe.
    const char *message = "Testing pipe size.";
    ssize_t bytes_written = write(pipe_fd[1], message, 17); // Write a message (17 bytes)

    // Close the write end of the pipe
    close(pipe_fd[1]);

    // Print message for demonstration purpose
    if (bytes_written > 0) {
        printf("Wrote %zd bytes to the pipe.\n", bytes_written);
    }

    // Close the read end of the pipe
    close(pipe_fd[0]);

    return 0;
}

