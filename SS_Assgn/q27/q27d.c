#include <stdio.h>
#include <unistd.h>

int main() {
    char *argv[] = { "ls", "-Rl", (char *)NULL };
    execv("/bin/ls", argv);
    // If execv is successful, the following line will not be executed
    perror("execv failed");
    return 1;
}

