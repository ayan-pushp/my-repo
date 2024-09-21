/*
============================================================================
Name : 27b.c
Author : Ayan Pushp
Description : Write a program to receive messages from the message queue.
a. with IPC_NOWAIT as a flag
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define MAX_TEXT 100

// Structure for message
struct message {
    long msg_type;       // Message type
    char msg_text[MAX_TEXT]; // Message text
};

int main() {
    // Generate a unique key for the message queue
    key_t key = ftok("msg_queue_file", 'B'); // Use a filename and an identifier
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Get the message queue ID
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct message msg;

    // Receive messages with non-blocking mode
    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, IPC_NOWAIT) == -1) {
            if (errno == ENOMSG) {
                printf("No message available (Non-blocking).\n");
                sleep(1); // Sleep for a second before checking again
                continue; // Continue to check for messages
            } else {
                perror("msgrcv");
                exit(EXIT_FAILURE);
            }
        }
        printf("Received (Non-blocking): %s\n", msg.msg_text);
    }

    return 0;
}

