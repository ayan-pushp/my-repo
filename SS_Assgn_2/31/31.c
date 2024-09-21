/*
============================================================================
Name : 31.c
Author : Ayan Pushp
Description : Write a program to create a semaphore and initialize value to the semaphore.
a. create a binary semaphore
b. create a counting semaphore
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define SEM_KEY 1234 // Semaphore key

// Function to initialize a semaphore
void init_semaphore(int semid, int semnum, int init_value) {
    if (semctl(semid, semnum, SETVAL, init_value) == -1) {
        perror("semctl SETVAL");
        exit(EXIT_FAILURE);
    }
    printf("Semaphore %d initialized to %d\n", semnum, init_value);
}

int main() {
    // Step 1: Create a binary semaphore
    int bin_semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (bin_semid == -1) {
        perror("semget binary");
        exit(EXIT_FAILURE);
    }
    // Initialize binary semaphore to 1 (available)
    init_semaphore(bin_semid, 0, 1);

    // Step 2: Create a counting semaphore
    int count_semid = semget(SEM_KEY + 1, 1, IPC_CREAT | 0666);
    if (count_semid == -1) {
        perror("semget counting");
        exit(EXIT_FAILURE);
    }
    // Initialize counting semaphore to 5
    init_semaphore(count_semid, 0, 5);

    // Step 3: Detach and remove semaphores (optional, for cleanup)
    if (semctl(bin_semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID binary");
        exit(EXIT_FAILURE);
    }
    if (semctl(count_semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID counting");
        exit(EXIT_FAILURE);
    }

    printf("Semaphores created and initialized successfully.\n");
    return 0;
}

