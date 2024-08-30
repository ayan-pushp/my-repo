#include <stdio.h>
#include <unistd.h>

int main() {
    execlp("ls", "ls", "-Rl", (char *)NULL);
    // If execlp is successful, the following line will not be executed
    perror("execlp failed");
    return 1;
}

