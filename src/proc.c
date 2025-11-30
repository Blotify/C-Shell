#include "proc.h"

void get_process_info(pid_t pid) {
    char path[256];
    char buffer[4096];
    char status;
    int vm_size;
    int pgid;
    int is_foreground = 0;

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    FILE *f = fopen(path, "r");
    if (!f) {
        return;
    }
    fscanf(f, "%*d %*s %c %d", &status, &pgid);
    fclose(f);

    snprintf(path, sizeof(path), "/proc/%d/stat", getpgrp());
    f = fopen(path, "r");
    if (f) {
        int fg;
        fscanf(f, "%*d %*s %*c %d", &fg);
        if (pgid == fg) {
            is_foreground = 1;
        }
        fclose(f);
    }

    snprintf(path, sizeof(path), "/proc/%d/statm", pid);
    f = fopen(path, "r");
    if (f) {
        fscanf(f, "%d", &vm_size);
        fclose(f);
    }

    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len >= 0) {
        buffer[len] = '\0';
    }

    printf("pid: %d\n", pid);
    printf("process status: %c%s\n", status, is_foreground ? "+" : "");
    printf("Process Group: %d\n", pgid);
    printf("Virtual Memory: %d KB\n", vm_size);
    printf("Executable Path: %s\n", buffer);
}

void process_proclore_command(char *command) {
    char *token = strtok(command, " ");
    token = strtok(NULL, " ");

    pid_t pid;
    if (token == NULL) {
        pid = getpid();
    } else {
        pid = atoi(token);
    }

    get_process_info(pid);
}
