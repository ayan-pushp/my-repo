/*
============================================================================
Name : 19d.c
Author : Ayan Pushp
Description : Create a FIFO file by
d. mkfifo library function
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main() {
    const char *fifo_name = "my_fifo";

    // Create a FIFO using mkfifo library function
    if (mkfifo(fifo_name, 0666) < 0) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    printf("FIFO created using mkfifo: %s\n", fifo_name);
    return EXIT_SUCCESS;
}

