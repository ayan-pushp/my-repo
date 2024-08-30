#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *exec = "./print_name";
    const char *input = "Ayan Pushp";
    
    char command[256];
    snprintf(command, sizeof(command), "%s %s", exec, input);

    int result = system(command);
    
    if (result == -1) {
        perror("system");
        return 1;
    }

    return 0;
}

