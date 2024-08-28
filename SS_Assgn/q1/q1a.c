#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {
    const char *target  = "/home/ayan/demo/my-repo";
    const char *link = "/home/ayan/myprog";

    // Creating symbolic link
    if (symlink(target, link) == -1) {
        // If symlink() fails, print error message
        perror("symlink");
        return EXIT_FAILURE;
    }

    printf("Symbolic link created successfully.\n");
    return EXIT_SUCCESS;
}
