#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    const char* filename = "my_file.txt";
    mode_t mode = 0644; // Permissions for the new file

    // Create a new empty file using creat()
    int fd = creat(filename, mode);

    if (fd != -1) {
        printf("File created successfully. File descriptor: %d\n", fd);
	printf("Pid:%d\n",getpid());
    }
    else {
        perror("Error creating file");
    }
	getchar();
    return 0;
}

