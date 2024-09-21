/*
============================================================================
Name : 28.c
Author : Ayan Pushp
Description : Write a program to change the exiting message queue permission. (use msqid_ds structure)
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

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

    // Structure to hold message queue information
    struct msqid_ds mq_info;

    // Retrieve the current message queue information
    if (msgctl(msgid, IPC_STAT, &mq_info) == -1) {
        perror("msgctl IPC_STAT");
        exit(EXIT_FAILURE);
    }

    // Print current permissions
    printf("Current Permissions: %o\n", mq_info.msg_perm.mode);

    // Change permissions: Setting to 0644 (rw-r--r--)
    mq_info.msg_perm.mode = 0644;

    // Update the message queue permissions
    if (msgctl(msgid, IPC_SET, &mq_info) == -1) {
        perror("msgctl IPC_SET");
        exit(EXIT_FAILURE);
    }

    // Retrieve the updated message queue information
    if (msgctl(msgid, IPC_STAT, &mq_info) == -1) {
        perror("msgctl IPC_STAT");
        exit(EXIT_FAILURE);
    }

    // Print updated permissions
    printf("Updated Permissions: %o\n", mq_info.msg_perm.mode);

    return 0;
}

