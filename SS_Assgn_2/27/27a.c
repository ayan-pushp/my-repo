/*
============================================================================
Name : 27a.c
Author : Ayan Pushp
Description : Write a program to receive messages from the message queue.
a. with 0 as a flag
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

    // Receive messages with blocking mode
    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        printf("Received (Blocking): %s\n", msg.msg_text);
    }

    return 0;
}

