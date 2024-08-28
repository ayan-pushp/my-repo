#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main() {

    const char *myfile = "/home/ayan/demo/my-repo/file1.txt";
    int fd = open(myfile, O_EXCL | O_RDWR, 0644);

    if (fd == -1) {
        if (errno == EEXIST) {
            printf("File already exists.\n");
        } else {
            perror("Error opening file");
        }
        exit(EXIT_FAILURE);
    }
    return 0;
}
