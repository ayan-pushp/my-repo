#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <time.h>

int main() {
    fd_set rfds;
    struct timeval tv;
    int result;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds); 

    tv.tv_sec = 10;
    tv.tv_usec = 0;

    printf("Waiting for input for 10 seconds...\n");

    result = select(1, &rfds, NULL, NULL, &tv);

    if (result == -1) {
        perror("select");
        exit(EXIT_FAILURE);
    } else if (result == 0) {
        printf("No input received within 10 seconds.\n");
    } else {
            printf("Input received within 10 seconds.\n");
        }
    return 0;
}

