/*
============================================================================
Name : 15.c
Author : Ayan Pushp
Description : Write a simple program to send some data from parent to the child process.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2]; // Array to hold the read and write file descriptors
    pid_t pid;
    char buffer[100]; // Buffer to store the data read from the pipe
    const char *message = "Hello, this message is from the parent!";

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Parent process (Writer)
        close(pipefd[0]); // Close the read end of the pipe
        write(pipefd[1], message, strlen(message) + 1); // Write message to the pipe
        printf("Parent: Wrote to pipe: %s\n", message);
        close(pipefd[1]); // Close the write end of the pipe
    } else {
        // Child process (Reader)
        close(pipefd[1]); // Close the write end of the pipe
        read(pipefd[0], buffer, sizeof(buffer)); // Read from the pipe
        close(pipefd[0]); // Close the read end of the pipe
        printf("Child: Read from pipe: %s\n", buffer);
    }

    return 0;
}

