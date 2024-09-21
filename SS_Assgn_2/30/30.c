/*
============================================================================
Name : 30.c
Author : Ayan Pushp
Description : Write a program to create a shared memory.
a. write some data to the shared memory
b. attach with O_RDONLY and check whether you are able to overwrite.
c. detach the shared memory
d. remove the shared memory
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024 // Size of shared memory segment

int main() {
    // Step 1: Create a unique key for the shared memory
    key_t key = ftok("shmfile", 'A'); // Use a filename and an identifier
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Step 2: Create a shared memory segment
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Step 3: Attach to the shared memory segment
    char *str = shmat(shmid, NULL, 0);
    if (str == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Step 4: Write data to shared memory
    strncpy(str, "Hello, Shared Memory!", SHM_SIZE);
    printf("Data written to shared memory: %s\n", str);

    // Step 5: Detach from the shared memory segment
    if (shmdt(str) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Step 6: Attach to the shared memory segment with read-only permission
    str = shmat(shmid, NULL, SHM_RDONLY);
    if (str == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Step 7: Attempt to overwrite data (this should fail)
    printf("Attempting to overwrite shared memory data...\n");
    strncpy(str, "This should not be written!", SHM_SIZE); // This may cause undefined behavior
    printf("Current data in shared memory: %s\n", str);

    // Step 8: Detach from the shared memory segment
    if (shmdt(str) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    // Step 9: Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID");
        exit(EXIT_FAILURE);
    }

    printf("Shared memory removed successfully.\n");
    return 0;
}

