#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <string.h>

void print_policy(int policy) {
    switch (policy) {
        case SCHED_FIFO:
            printf("Current policy: SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("Current policy: SCHED_RR\n");
            break;
        default:
            printf("Current policy: Unknown\n");
            break;
    }
}

int main(int argc, char *argv[]) {
    int policy;
    struct sched_param param;
    policy = sched_getscheduler(0);
    if (policy == -1) {
        perror("sched_getscheduler");
        return EXIT_FAILURE;
    }

    print_policy(policy);
    if (argc > 1) {
        int new_policy = -1;
        if (strcmp(argv[1], "SCHED_FIFO") == 0) {
            new_policy = SCHED_FIFO;
        } else if (strcmp(argv[1], "SCHED_RR") == 0) {
            new_policy = SCHED_RR;
        } else {
            fprintf(stderr, "Unknown policy: %s\n", argv[1]);
            return EXIT_FAILURE;
        }
        
        param.sched_priority = 10; 
        if (sched_setscheduler(0, new_policy, &param) == -1) {
            perror("sched_setscheduler");
            return EXIT_FAILURE;
        }
        policy = sched_getscheduler(0);
        if (policy == -1) {
            perror("sched_getscheduler");
            return EXIT_FAILURE;
        }
        print_policy(policy);
    }

    return EXIT_SUCCESS;
}

