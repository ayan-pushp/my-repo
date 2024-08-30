#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILE_PATH "/home/ayan/demo/my-repo/file1.txt"  
void write_data_to_file(const char *file_path) {
    int fd;
    ssize_t bytes_written;
    off_t offset;
    fd = open(file_path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    const char *data1 = "1234567890";
    bytes_written = write(fd, data1, 10);
    if (bytes_written != 10) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Wrote 10 bytes of data.\n");
    offset = lseek(fd, 10, SEEK_CUR);
    if (offset == -1) {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Moved file pointer by 10 bytes, new offset: %ld\n", (long)offset);
    const char *data2 = "ABCDEFGHIJ";
    bytes_written = write(fd, data2, 10);
    if (bytes_written != 10) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Wrote another 10 bytes of data.\n");

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    printf("File closed.\n");
}

int main() {
    write_data_to_file(FILE_PATH);
    return 0;
}

