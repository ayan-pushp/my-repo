#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {
    const char *existing_file_path = "home/ayan/demo/my-repo/file2.txt";
    const char *hard_link_path = "/home/ayan/Desktop/mydemo.txt";

    // Creating hard link
    if (link(existing_file_path, hard_link_path) == -1) {
        // If link() fails, print an error message
        perror("link");
        return EXIT_FAILURE;
    }

    printf("Hard link created successfully.\n");
    return EXIT_SUCCESS;
}
