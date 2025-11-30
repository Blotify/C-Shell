#include "fgbg.h"

void process_fg_command(const char *command) {
    int pid;
    sscanf(command, "fg %d", &pid);

    int found = 0;
    for (int i = 0; i < MAX_PROCS; i++) {
        if (processes[i].pid == pid && processes[i].pid != shell_pid) {
            found = 1;

            tcsetpgrp(STDIN_FILENO, pid);
            if (processes[i].state == 2) {
                kill(pid, SIGCONT);
            }
            processes[i].state = 0;

            int status;
            struct timeval start, end;

            set_foreground_pid(pid);
            gettimeofday(&start, NULL);
            waitpid(pid, &status, WUNTRACED);
            gettimeofday(&end, NULL);
            set_foreground_pid(0);

            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                processes[i].state = 1;
                processes[i].end_time = time(NULL);
            } else if (WIFSTOPPED(status)) {
                processes[i].state = 2;
            }

            time_monitor(start, end, "fg");

            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(STDIN_FILENO, shell_pid);
            signal(SIGTTOU, SIG_DFL);

            return;
        }
    }

    if (!found) {
        printf("No such process found\n");
    }
}

void process_bg_command(const char *command) {
    int pid;
    sscanf(command, "bg %d", &pid);

    int found = 0;
    for (int i = 0; i < MAX_PROCS; i++) {
        if (processes[i].pid == pid) {
            found = 1;
            if (processes[i].state == 2) {
                kill(pid, SIGCONT);
                processes[i].state = 1;
                printf("Process %d resumed\n", pid);
            } else {
                printf("Process %d already running\n", pid);
            }
            break;
        }
    }

    if (!found) {
        printf("No such process found\n");
    }
}
