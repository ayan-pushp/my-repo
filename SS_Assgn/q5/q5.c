#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
     const char *file[5] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt", "file5.txt"};
    int fd[5],i; 

    while (1) {
        for (i = 0; i < 5; i++) {
            fd[i] = open(file[i], O_RDWR | O_CREAT | O_TRUNC, 0666);
            if (fd[i] == -1) {
                perror("Error");
                exit(1);
            }
            printf("File name: %s File descriptor: %d\n", file[i], fd[i]);
        }
        sleep(10);
        for (i = 0; i < 5; i++) close(fd[i]);
        sleep(1); 
    }
return 0;
}
