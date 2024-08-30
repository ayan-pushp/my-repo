#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>

int main() {
    int policy;
    int max_priority, min_priority;
    int policies[] = {SCHED_FIFO, SCHED_RR};
    const char *policy_names[] = {"SCHED_FIFO", "SCHED_RR"};

    for (int i = 0; i < sizeof(policies) / sizeof(policies[0]); i++) {
        policy = policies[i];

        max_priority = sched_get_priority_max(policy);
        if (max_priority == -1) {
            perror("sched_get_priority_max");
            exit(EXIT_FAILURE);
        }

        min_priority = sched_get_priority_min(policy);
        if (min_priority == -1) {
            perror("sched_get_priority_min");
            exit(EXIT_FAILURE);
        }

        printf("Policy: %s\n", policy_names[i]);
        printf("  Max Priority: %d\n", max_priority);
        printf("  Min Priority: %d\n", min_priority);
    }

    return 0;
}

