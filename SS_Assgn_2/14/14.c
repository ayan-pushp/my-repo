/*
============================================================================
Name : 14.c
Author : Ayan Pushp
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2]; // Array to hold the read and write file descriptors
    char buffer[100]; // Buffer to store the data read from the pipe
    const char *message = "Hello, this is a message from the writer!";

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Write to the pipe
    write(pipefd[1], message, strlen(message) + 1); // Include null terminator
    close(pipefd[1]); // Close the write end of the pipe

    // Read from the pipe
    read(pipefd[0], buffer, sizeof(buffer)); // Read into buffer
    close(pipefd[0]); // Close the read end of the pipe

    // Display the message read from the pipe
    printf("Read from pipe: %s\n", buffer);

    return 0;
}

