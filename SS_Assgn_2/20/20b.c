/*
============================================================================
Name : 20b.c
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
    char buffer[100];

    int fd = open(FIFO_NAME, O_RDONLY); // Open FIFO for reading
    read(fd, buffer, sizeof(buffer)); // Read message from FIFO
    printf("Child: Received from FIFO: %s\n", buffer);
    close(fd); // Close FIFO

    // Optionally remove the FIFO after use
    unlink(FIFO_NAME);

    return 0;
}

