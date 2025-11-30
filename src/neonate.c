#include "neonate.h"

long long get_process_start_time(pid_t pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);

    FILE *f = fopen(path, "r");
    if (!f) {
        return -1;
    }

    char buffer[1024];
    long long start_time = -1;
    if (fgets(buffer, sizeof(buffer), f)) {
        char *token = strtok(buffer, " ");
        for (int i = 1; i < 22 && token; i++) {
            token = strtok(NULL, " ");
        }
        if (token) {
            start_time = atoll(token);
        }
    }
    fclose(f);
    return start_time;
}

pid_t get_most_recent_pid() {
    DIR *dir = opendir("/proc");
    if (!dir) {
        return -1;
    }

    struct dirent *entry;
    pid_t recent_pid = -1;
    long long recent_start_time = -1;

    while ((entry = readdir(dir)) != NULL) {
        int is_pid = 1;
        for (int i = 0; entry->d_name[i]; i++) {
            if (!isdigit(entry->d_name[i])) {
                is_pid = 0;
                break;
            }
        }

        if (is_pid) {
            pid_t pid = atoi(entry->d_name);
            long long start_time = get_process_start_time(pid);
            if (start_time > recent_start_time) {
                recent_start_time = start_time;
                recent_pid = pid;
            }
        }
    }
    closedir(dir);
    return recent_pid;
}

int is_x_key_pressed() {
    struct termios old_term, new_term;
    int ch;

    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    fd_set read_fds;
    struct timeval tv;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    int ret = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &tv);
    if (ret > 0 && FD_ISSET(STDIN_FILENO, &read_fds)) {
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        if (ch == 'x' || ch == 'X') {
            return 1;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    return 0;
}

void process_neonate_command(const char *command) {
    if (strstr(command, "-n") == NULL) {
        fprintf(stderr, "Error: no -n flag\n");
        return;
    }

    int time_arg;
    sscanf(command, "neonate -n %d", &time_arg);

    while (1) {
        pid_t pid = get_most_recent_pid();
        if (pid == -1) {
            fprintf(stderr, "Error\n");
            return;
        }
        printf("%d\n", pid);
        fflush(stdout);
        sleep(time_arg);
        if (is_x_key_pressed()) {
            break;
        }
    }
}
