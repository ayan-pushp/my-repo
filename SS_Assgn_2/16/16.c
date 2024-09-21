/*
============================================================================
Name : 16.c
Author : Ayan Pushp
Description : Write a program to send and receive data from parent to child vice versa. Use two way communication.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe_parent_to_child[2]; // Pipe for parent to child
    int pipe_child_to_parent[2]; // Pipe for child to parent
    pid_t pid;

    // Create the pipes
    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
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
        // Parent process
        close(pipe_parent_to_child[0]); // Close read end of parent-to-child pipe
        close(pipe_child_to_parent[1]); // Close write end of child-to-parent pipe

        const char *parent_message = "Hello from Parent!";
        write(pipe_parent_to_child[1], parent_message, strlen(parent_message) + 1); // Send message to child
        printf("Parent: Sent to child: %s\n", parent_message);

        char buffer[100];
        read(pipe_child_to_parent[0], buffer, sizeof(buffer)); // Read message from child
        printf("Parent: Received from child: %s\n", buffer);

        close(pipe_parent_to_child[1]); // Close write end of parent-to-child pipe
        close(pipe_child_to_parent[0]); // Close read end of child-to-parent pipe
    } else {
        // Child process
        close(pipe_parent_to_child[1]); // Close write end of parent-to-child pipe
        close(pipe_child_to_parent[0]); // Close read end of child-to-parent pipe

        char buffer[100];
        read(pipe_parent_to_child[0], buffer, sizeof(buffer)); // Read message from parent
        printf("Child: Received from parent: %s\n", buffer);

        const char *child_message = "Hello from Child!";
        write(pipe_child_to_parent[1], child_message, strlen(child_message) + 1); // Send message to parent
        printf("Child: Sent to parent: %s\n", child_message);

        close(pipe_parent_to_child[0]); // Close read end of parent-to-child pipe
        close(pipe_child_to_parent[1]); // Close write end of child-to-parent pipe
    }

    return 0;
}

