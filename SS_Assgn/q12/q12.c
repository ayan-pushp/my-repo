#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void open_mode(int flags) {
    if (flags & O_RDONLY) {
        printf("Read-Only\n");
    }
    if (flags & O_WRONLY) {
        printf("Write-Only\n");
    }
    if (flags & O_RDWR) {
        printf("Read-Write\n");
    }
}

int main() {
    const char *file_path = "/home/ayan/demo/my-repo/file2.txt";
    int fd = open(file_path, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        perror("fcntl");
        close(fd);
        return 1;
    }
    printf("File '%s' opened with modes:\n", file_path);
    open_mode(flags);
    close(fd);
    return 0;
}

