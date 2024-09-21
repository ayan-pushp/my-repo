/*
============================================================================
Name : 32.c
Author : Ayan Pushp
Description : Write a program to implement semaphore to protect any critical section.
a. rewrite the ticket number creation program using semaphore
b. protect shared memory from concurrent write access
c. protect multiple pseudo resources ( may be two) using counting semaphore
d. remove the created semaphore
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SEM_KEY 1234 // Semaphore key
#define SHM_SIZE 1024 // Size of shared memory segment
#define NUM_RESOURCES 2 // Number of pseudo resources

// Function to initialize a semaphore
void init_semaphore(int semid, int semnum, int init_value) {
    if (semctl(semid, semnum, SETVAL, init_value) == -1) {
        perror("semctl SETVAL");
        exit(EXIT_FAILURE);
    }
}

// Function to perform P (wait) operation on semaphore
void sem_wait(int semid) {
    struct sembuf sops = {0, -1, 0}; // Decrement the semaphore
    if (semop(semid, &sops, 1) == -1) {
        perror("semop wait");
        exit(EXIT_FAILURE);
    }
}

// Function to perform V (signal) operation on semaphore
void sem_signal(int semid) {
    struct sembuf sops = {0, 1, 0}; // Increment the semaphore
    if (semop(semid, &sops, 1) == -1) {
        perror("semop signal");
        exit(EXIT_FAILURE);
    }
}

// Ticket number creation function
void create_ticket_number(int semid) {
    for (int i = 0; i < 5; i++) {
        sem_wait(semid); // Enter critical section
        printf("Ticket Number: %d\n", i + 1);
        sleep(1); // Simulate some processing time
        sem_signal(semid); // Exit critical section
    }
}

// Shared memory access function
void shared_memory_access(int semid) {
    int shmid = shmget(SEM_KEY + 1, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    
    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; i++) {
        sem_wait(semid); // Protect shared memory
        sprintf(shm_ptr, "Data %d", i + 1);
        printf("Written to shared memory: %s\n", shm_ptr);
        sleep(1); // Simulate some processing time
        sem_signal(semid); // Release shared memory
    }

    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

// Protect multiple resources using counting semaphore
void protect_multiple_resources(int semid) {
    for (int i = 0; i < 5; i++) {
        sem_wait(semid); // Wait for resources
        printf("Using resource %d\n", i % NUM_RESOURCES + 1);
        sleep(1); // Simulate using the resource
        sem_signal(semid); // Release resource
    }
}

int main() {
    // Step 1: Create a semaphore
    int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphore for ticket number creation (binary semaphore)
    init_semaphore(semid, 0, 1);

    // Ticket number creation
    create_ticket_number(semid);

    // Initialize semaphore for shared memory access (binary semaphore)
    init_semaphore(semid, 0, 1);

    // Shared memory access
    shared_memory_access(semid);

    // Initialize counting semaphore for resource protection
    init_semaphore(semid, 0, NUM_RESOURCES);

    // Protect multiple resources
    protect_multiple_resources(semid);

    // Step 2: Remove the semaphore
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID");
        exit(EXIT_FAILURE);
    }

    printf("Semaphore removed successfully.\n");
    return 0;
}

