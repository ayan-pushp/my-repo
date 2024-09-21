/*
============================================================================
Name : 19c.c
Author : Ayan Pushp
Description : Create a FIFO file by
a. mknod system call
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    const char *fifo_name = "my_fifo";

    // Create a FIFO using mknod system call
    if (mknod(fifo_name, S_IFIFO | 0666, 0) < 0) {
        perror("mknod");
        return EXIT_FAILURE;
    }

    printf("FIFO created using mknod: %s\n", fifo_name);
    return EXIT_SUCCESS;
}

