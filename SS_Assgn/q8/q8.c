#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_PATH "/home/ayan/demo/my-repo/file1.txt" 

int main() {
    int fd;
    ssize_t bytesRead;
    char buffer[1024];
    char *newline;

    fd = open(FILE_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (write(STDOUT_FILENO, &buffer[i], 1) == -1) {
                perror("Error writing to stdout");
                close(fd);
                return EXIT_FAILURE;
            }
            if (buffer[i] == '\n') {
                break;
            }
        }
    }
    if (bytesRead == -1) {
        perror("Error reading file");
    }
    if (close(fd) == -1) {
        perror("Error closing file");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

