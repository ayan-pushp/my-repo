#include <stdio.h>
#include <unistd.h>

int main() {
    char *envp[] = { NULL };  // Environment variables
    execle("/bin/ls", "ls", "-Rl", (char *)NULL, envp);
    // If execle is successful, the following line will not be executed
    perror("execle failed");
    return 1;
}

