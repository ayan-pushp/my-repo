#include <stdio.h>
#include <unistd.h>

int main() {
    char buffer[1024];
    ssize_t bytesRead = read(0, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("Error reading from stdin");
        return 1;
    }
    ssize_t bytesWritten = write(1, buffer, bytesRead);
    if (bytesWritten == -1) {
        perror("Error writing to stdout");
        return 1;
    }
   return 0;
}
