/*
============================================================================
Name : 6.c
Author : Ayan Pushp
Description : Write a simple program to create three threads
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3

// Function that each thread will execute
void* thread_function(void* arg) {
    int thread_num = *((int*)arg);
    printf("Hello from thread %d!\n", thread_num);
    free(arg);  // Free the dynamically allocated memory
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];  // Array to hold thread identifiers

    for (int i = 0; i < NUM_THREADS; i++) {
        int* thread_num = malloc(sizeof(int));  // Allocate memory for thread number
        if (thread_num == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        *thread_num = i + 1;  // Set thread number (1-based)

        // Create the thread
        if (pthread_create(&threads[i], NULL, thread_function, thread_num) != 0) {
            perror("Failed to create thread");
            free(thread_num);  // Free the allocated memory on failure
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads have finished execution.\n");
    return 0;
}

