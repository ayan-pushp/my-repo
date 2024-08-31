/*
============================================================================
Name : 20.c
Author : Ayan Pushp
Description : Find out the priority of your running program. Modify the priority with nice command.
Date : 30th Aug 2024
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
int main() {
	pid_t pid = getpid();
	int priority = getpriority(PRIO_PROCESS, pid);
	if(priority == -1) {
		perror("Error fetching priority \n");
		return 1;
	}
	printf("Current priority: %d\n", priority);
	int new_val = 5;
	int new_priority = nice(new_val);
	if (new_priority == -1) {
        	perror("Error setting new priority");
        	return 1;
    	}
	priority = getpriority(PRIO_PROCESS, pid);
	if(priority == -1) {
		perror("Error fetching priority \n");
		return 1;
	}
	printf("New priority: %d\n", priority);

}
