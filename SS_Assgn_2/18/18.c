/*
============================================================================
Name : 18.c
Author : Ayan Pushp
Description : Write a program to find out total number of directories on the pwd.
execute ls -l | grep ^d | wc ? Use only dup2
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
    int fd1[2], fd2[2];
    pid_t pid1, pid2, pid3;

    // Create two pipes
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // First child process for `ls -l`
    if ((pid1 = fork()) == 0) {
        dup2(fd1[1], STDOUT_FILENO); // Redirect stdout to first pipe
        close(fd1[0]); // Close read end of first pipe
        close(fd1[1]); // Close the write end (already duplicated)
        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }

    // Second child process for `grep ^d`
    if ((pid2 = fork()) == 0) {
        dup2(fd1[0], STDIN_FILENO); // Redirect stdin to read end of first pipe
        dup2(fd2[1], STDOUT_FILENO); // Redirect stdout to second pipe
        close(fd1[1]); // Close write end of first pipe
        close(fd1[0]); // Close read end of first pipe (already duplicated)
        close(fd2[0]); // Close read end of second pipe
        execlp("grep", "grep", "^d", NULL);
        perror("execlp grep");
        exit(EXIT_FAILURE);
    }

    // Third child process for `wc`
    if ((pid3 = fork()) == 0) {
        dup2(fd2[0], STDIN_FILENO); // Redirect stdin to read end of second pipe
        close(fd2[1]); // Close write end of second pipe
        close(fd1[0]); // Close read end of first pipe
        close(fd1[1]); // Close write end of first pipe
        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc");
        exit(EXIT_FAILURE);
    }

    // Close all pipe ends in the parent process
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    // Wait for all child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    return 0;
}

