#include <stdio.h>

extern char **environ;

int main() {
    char **e = environ;
    while (*e) {
        printf("%s\n", *e);
        e++;
    }
    return 0;
}

