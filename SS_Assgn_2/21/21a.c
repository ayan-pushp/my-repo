/*
============================================================================
Name : 21a.c
Author : Ayan Pushp
Description : Write two programs so that both can communicate by FIFO -Use two way communication.
Date: 20th Sep, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_TO_CHILD "fifo_to_child"
#define FIFO_TO_PARENT "fifo_to_parent"

int main() {
    // Create FIFOs
    mkfifo(FIFO_TO_CHILD, 0666);
    mkfifo(FIFO_TO_PARENT, 0666);

    const char *parent_message = "Hello from Parent!";
    int fd_to_child = open(FIFO_TO_CHILD, O_WRONLY);
    write(fd_to_child, parent_message, strlen(parent_message) + 1);
    printf("Parent: Sent to child: %s\n", parent_message);
    close(fd_to_child);

    char buffer[100];
    int fd_to_parent = open(FIFO_TO_PARENT, O_RDONLY);
    read(fd_to_parent, buffer, sizeof(buffer));
    printf("Parent: Received from child: %s\n", buffer);
    close(fd_to_parent);

    // Clean up FIFOs
    unlink(FIFO_TO_CHILD);
    unlink(FIFO_TO_PARENT);

    return 0;
}

