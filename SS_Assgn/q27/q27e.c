#include <stdio.h>
#include <unistd.h>

int main() {
    char *argv[] = { "ls", "-Rl", (char *)NULL };
    execvp("ls", argv);
    // If execvp is successful, the following line will not be executed
    perror("execvp failed");
    return 1;
}

