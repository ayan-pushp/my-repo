/*
============================================================================
Name : 26.c
Author : Ayan Pushp
Description : Write a program to send messages to the message queue. Check $ipcs -q
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

    // Create a message queue
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct message msg;
    msg.msg_type = 1; // Set message type

    // Send messages to the queue
    for (int i = 0; i < 5; i++) {
        snprintf(msg.msg_text, MAX_TEXT, "Message %d", i + 1);
        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        printf("Sent: %s\n", msg.msg_text);
        sleep(1); // Sleep for a second between messages
    }

    // Cleanup: Optionally, you can remove the message queue
    // Uncomment the following line if you want to remove the queue after sending messages
    // msgctl(msgid, IPC_RMID, NULL);

    return 0;
}

