#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int desc = open("q11.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (desc < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    int desc_dup = dup(desc);
    if (desc_dup < 0) {
        perror("failed dup");
        close(desc);
        exit(EXIT_FAILURE);
    }
    int desc_dup2 = dup2(desc, 10); //10 is the number we have given as which fd we want
    if (desc_dup2 < 0) {
        perror("fail dup2");
        close(desc);
        close(desc_dup);
        exit(EXIT_FAILURE);
    }
    int desc_fcntl = fcntl(desc, F_DUPFD, 11);
    if (desc_fcntl < 0) {
        perror("fail fcntl");
        close(desc);
        close(desc_dup);
        close(desc_dup2);
        exit(EXIT_FAILURE);
    }
 const char *mess[] = { "old one FD\n","dup1 FD\n","dup2 FD\n",
        "fcntl FD\n"
    };
    write(desc, mess[0], strlen(mess[0]));
    write(desc_dup, mess[1], strlen(mess[1]));
    write(desc_dup2, mess[2], strlen(mess[2]));
    write(desc_fcntl, mess[3], strlen(mess[3]));
    
    close(desc);
    close(desc_dup);
    close(desc_dup2);
    close(desc_fcntl);
    
    FILE *input = fopen("q11.txt", "r");
    if (input) {
        char line[256];
        printf("contents:\n");
        while (fgets(line, sizeof(line), input)) {
            printf("%s", line);}fclose(input);} 
      else {
        perror("fail to open");
    }

    return 0;
}
