#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

void print_file_info(const char *file_path) {
    struct stat file_stat;
    
    if (stat(file_path, &file_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("File: %s\n", file_path);
    printf("Inode: %ld\n", (long)file_stat.st_ino);
    printf("Number of Hard Links: %ld\n", (long)file_stat.st_nlink);
    printf("UID: %d (%s)\n", file_stat.st_uid, getpwuid(file_stat.st_uid)->pw_name);
    printf("GID: %d (%s)\n", file_stat.st_gid, getgrgid(file_stat.st_gid)->gr_name);
    printf("Size: %ld bytes\n", (long)file_stat.st_size);
    printf("Block Size: %ld bytes\n", (long)file_stat.st_blksize);
    printf("Number of Blocks: %ld\n", (long)file_stat.st_blocks);
    
    printf("Time of Last Access: %s", ctime(&file_stat.st_atime));
    printf("Time of Last Modification: %s", ctime(&file_stat.st_mtime));
    printf("Time of Last Change: %s", ctime(&file_stat.st_ctime));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    print_file_info(argv[1]);
    return 0;
}

