#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
    const char *fifo_path = "/home/ayan/myfiles";

    //Creating FIFO file
    if (mkfifo(fifo_path, 0666) == -1) {
        //If mkfifo() fails, printing error message
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    printf("FIFO file created successfully.\n");
    return EXIT_SUCCESS;
}

