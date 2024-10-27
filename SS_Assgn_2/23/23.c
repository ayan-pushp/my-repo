/*
============================================================================
Name : 23.c
Author : Ayan Pushp
Description : Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    long PIPE_BUF, OPEN_MAX;
    PIPE_BUF=pathconf(".",_PC_PIPE_BUF);
    OPEN_MAX=sysconf(_SC_OPEN_MAX);
    printf("Pipe Size=%ld\t Max Files=%ld",PIPE_BUF,OPEN_MAX);

    return 0;
}

