#include <stdio.h>
#include <unistd.h>

int main() {
    execl("/bin/ls", "ls", "-Rl", (char *)NULL);
    // If execl is successful, the following line will not be executed
    perror("execl failed");
    return 1;
}

