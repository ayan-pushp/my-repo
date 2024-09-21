/*
============================================================================
Name : 22.c
Author : Ayan Pushp
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>

#define FIFO_NAME "my_fifo"

int main() {
    // Create FIFO
    mkfifo(FIFO_NAME, 0666);

    char buffer[100];
    int fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK); // Open FIFO for reading

    fd_set readfds;
    struct timeval timeout;
    
    while (1) {
        // Clear the set and add the FIFO file descriptor
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        // Set timeout to 10 seconds
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        // Wait for data to be available
        int retval = select(fd + 1, &readfds, NULL, NULL, &timeout);

        if (retval == -1) {
            perror("select()");
            break;
        } else if (retval == 0) {
            printf("No data within 10 seconds.\n");
            break; // Timeout occurred
        } else {
            // Data is available to read
            if (FD_ISSET(fd, &readfds)) {
                ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0'; // Null-terminate the string
                    printf("Received from FIFO: %s\n", buffer);
                }
            }
        }
    }

    close(fd);
    unlink(FIFO_NAME); // Clean up FIFO
    return 0;
}

