/*
============================================================================
Name : 17a.c
Author : Ayan Pushp
Description : Write a program to execute ls -l | wc.
a. use dup
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pid_t pid1, pid2;

    // Create a pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // First child process for `ls -l`
    if ((pid1 = fork()) == 0) {
        // Redirect stdout to pipe
        dup(fd[1]); // Duplicate fd[1] to stdout
        close(fd[0]); // Close read end of the pipe
        close(fd[1]); // Close the write end of the pipe
        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }

    // Second child process for `wc`
    if ((pid2 = fork()) == 0) {
        // Redirect stdin to pipe
        dup(fd[0]); // Duplicate fd[0] to stdin
        close(fd[1]); // Close write end of the pipe
        close(fd[0]); // Close the read end of the pipe
        execlp("wc", "wc", NULL);
        perror("execlp wc");
        exit(EXIT_FAILURE);
    }

    // Close the pipe in the parent
    close(fd[0]);
    close(fd[1]);

    // Wait for both child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}

