/*
============================================================================
Name : 20a.c
Author : Ayan Pushp
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "my_fifo"

int main() {
    // Create the FIFO (named pipe)
    mkfifo(FIFO_NAME, 0666);

    const char *message = "Hello from Parent!";
    int fd = open(FIFO_NAME, O_WRONLY); // Open FIFO for writing
    write(fd, message, strlen(message) + 1); // Write message to FIFO
    printf("Parent: Sent to FIFO: %s\n", message);
    close(fd); // Close FIFO

    return 0;
}

