/*
============================================================================
Name : 21b.c
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
    char buffer[100];

    int fd_to_child = open(FIFO_TO_CHILD, O_RDONLY);
    read(fd_to_child, buffer, sizeof(buffer));
    printf("Child: Received from parent: %s\n", buffer);
    close(fd_to_child);

    const char *child_message = "Hello from Child!";
    int fd_to_parent = open(FIFO_TO_PARENT, O_WRONLY);
    write(fd_to_parent, child_message, strlen(child_message) + 1);
    printf("Child: Sent to parent: %s\n", child_message);
    close(fd_to_parent);

    return 0;
}

