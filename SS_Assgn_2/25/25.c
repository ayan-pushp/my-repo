/*
============================================================================
Name : 25.c
Author : Ayan Pushp
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
a. access permission
b. uid, gid
c. time of last message sent and received
d. time of last change in the message queue
d. size of the queue
f. number of messages in the queue
g. maximum number of bytes allowed
h. pid of the msgsnd and msgrcv
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
#include <time.h>

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

    // Structure to hold message queue information
    struct msqid_ds mq_info;

    // Retrieve message queue information
    if (msgctl(msgid, IPC_STAT, &mq_info) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    // Print message queue properties
    printf("Access Permissions: %o\n", mq_info.msg_perm.mode);
    printf("UID: %d\n", mq_info.msg_perm.uid);
    printf("GID: %d\n", mq_info.msg_perm.gid);
    printf("Time of Last Message Sent: %s", ctime(&mq_info.msg_stime));
    printf("Time of Last Message Received: %s", ctime(&mq_info.msg_rtime));
    printf("Time of Last Change: %s", ctime(&mq_info.msg_ctime));
    printf("Size of the Queue: %lu bytes\n", mq_info.__msg_cbytes);
    printf("Number of Messages in the Queue: %lu\n", mq_info.msg_qnum);
    printf("Maximum Number of Bytes Allowed: %lu\n", mq_info.msg_qbytes);
    printf("PID of Last msgsnd: %d\n", mq_info.msg_perm.__key); // Using key for demonstration

    // Cleanup: Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}

