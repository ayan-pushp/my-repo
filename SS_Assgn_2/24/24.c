/*
============================================================================
Name : 24.c
Author : Ayan Pushp
Description : Write a program to create a message queue and print the key and message queue id.
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

int main() {
    // Generate a unique key for the message queue
    key_t key = ftok("msg_queue_file", 'B'); // Use a filename and an identifier
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Create a message queue
    int msgid = msgget(key, 0666 | IPC_CREAT); // Create message queue with read/write permissions
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Print the key and message queue ID
    printf("Message Queue Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);

    // Cleanup: Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}

