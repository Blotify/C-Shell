#include "ping.h"

void process_ping_command(char *command) {
    char *token;
    char *rest = command;

    strtok_r(rest, " ", &rest);
    token = strtok_r(rest, " ", &rest);
    if (!token) {
        fprintf(stderr, "Invalid ping\n");
        return;
    }
    pid_t pid = atoi(token);

    token = strtok_r(rest, " ", &rest);
    if (!token) {
        fprintf(stderr, "Invalid ping\n");
        return;
    }
    int signal_number = atoi(token) % 32;

    if (kill(pid, 0) == -1 && errno == ESRCH) {
        fprintf(stderr, "No such process\n");
        return;
    }

    if (kill(pid, signal_number) == 0) {
        printf("Sent signal %d to pid %d\n", signal_number, pid);
    } else {
        perror("signal");
    }
}
