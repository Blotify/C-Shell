#include "keyboard.h"
#include "spec6.h"

volatile pid_t foreground_pid = 0;

void handle_sigint(int sig) {
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGINT);
        printf("\nInterrupted %d\n", foreground_pid);
        for (int i = 0; i < MAX_PROCS; i++) {
            if (processes[i].pid == foreground_pid) {
                processes[i].state = 1;
                processes[i].end_time = time(NULL);
                break;
            }
        }
        foreground_pid = 0;
    }
}

void handle_sigtstp(int sig) {
    if (foreground_pid > 0 && foreground_pid != shell_pid) {
        kill(foreground_pid, SIGTSTP);
        printf("\nStopped %d\n", foreground_pid);
        for (int i = 0; i < MAX_PROCS; i++) {
            if (processes[i].pid == foreground_pid) {
                processes[i].state = 2;
                break;
            }
        }
        setpgid(foreground_pid, foreground_pid);
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(STDIN_FILENO, shell_pid);
        signal(SIGTTOU, SIG_DFL);
        foreground_pid = 0;
    }
}

void handle_sigquit(int sig) {
    printf("\nLogging out...\n");
    for (int i = 0; i < MAX_PROCS; i++) {
        if (processes[i].pid != 0) {
            kill(processes[i].pid, SIGKILL);
        }
    }
    exit(0);
}

void setup_keyboard_handlers() {
    struct sigaction sa_int, sa_tstp, sa_quit;

    sa_int.sa_handler = handle_sigint;
    sigfillset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);

    sa_tstp.sa_handler = handle_sigtstp;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa_tstp, NULL);

    sa_quit.sa_handler = handle_sigquit;
    sigfillset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void set_foreground_pid(pid_t pid) {
    foreground_pid = pid;
}
