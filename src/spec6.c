#include "spec6.h"
ProcessInfo processes[MAX_PROCS] = {0};
void handle_sigchld(int sig) {
    int status; pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        for (int i = 0; i < MAX_PROCS; i++) {
            if (processes[i].pid == pid) {
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    processes[i].state = 1; processes[i].end_time = time(NULL);
                    printf("%s ended (%d)\n", processes[i].name, pid);
                } else if (WIFSTOPPED(status)) {
                    processes[i].state = 2;
                    printf("%s stopped (%d)\n", processes[i].name, pid);
                }
                break;
            }
        }
    }
}
int add_process(pid_t pid, const char* name, int is_background) {
    for (int i = 0; i < MAX_PROCS; i++) {
        if (processes[i].pid == 0) {
            processes[i].name = strdup(name); processes[i].pid = pid;
            processes[i].state = 0; processes[i].end_time = 0; return i;
        }
    } return -1;
}
void time_monitor(struct timeval start, struct timeval end, char* command) {
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)*1e-6;
    long t = lround(elapsed);
    if (t > 2) printf("%s: %ld seconds\n", command, t);
}
void syscommands(char* command, int flag) {
    char *args[100], command_copy[256];
    strncpy(command_copy, command, sizeof(command_copy));
    char *token = strtok(command_copy, " \t"); int i = 0;
    while (token && i < 99) { args[i] = token; i++; token = strtok(NULL, " \t"); }
    args[i] = NULL;
    struct timeval start, end;
    int rc = fork();
    if (rc < 0) { printf("fork failed\n"); exit(1); }
    else if (rc == 0) {
        if (flag) setpgrp();
        execvp(args[0], args);
        fprintf(stderr, "ERROR: '%s' is not a valid command\n", args[0]);
        exit(127);
    } else {
        int idx = add_process(rc, args[0], flag);
        if (!flag) {
            gettimeofday(&start, NULL);
            int status; waitpid(rc, &status, WUNTRACED);
            gettimeofday(&end, NULL);
            if (idx != -1) {
                if (WIFEXITED(status) || WIFSIGNALED(status)) { processes[idx].state = 1; processes[idx].end_time = time(NULL); }
                else if (WIFSTOPPED(status)) processes[idx].state = 2;
            }
            time_monitor(start, end, args[0]);
        } else { printf("[%d] %d\n", rc, rc); }
    }
}
