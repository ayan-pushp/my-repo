/*
============================================================================
Name : 7.c
Author : Ayan Pushp
Description : Write a simple program to print the created thread ids.
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3

// Function that each thread will execute
void* thread_function(void* arg) {
    pthread_t thread_id = pthread_self();  // Get the thread ID
    printf("Hello from thread ID: %lu\n", (unsigned long)thread_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];  // Array to hold thread identifiers

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            perror("Failed to create thread");
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

