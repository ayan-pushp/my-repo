/*
============================================================================
Name : 29.c
Author : Ayan Pushp
Description : Write a program to remove the message queue.
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID");
        exit(EXIT_FAILURE);
    }

    printf("Message queue removed successfully.\n");
    return 0;
}

