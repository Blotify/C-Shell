#include "log.h"
#include "spec2.h"

void add_to_log(const char *command, const char *home) {
    char file_path[MAX_PATH_LEN];
    snprintf(file_path, sizeof(file_path), "%s/newfile.txt", home);

    char *commands[MAX_COMMANDS];
    int command_count = 0;
    char *last_command = NULL;

    int fd = open(file_path, O_RDONLY);
    if (fd >= 0) {
        char buffer[MAX_LINE_LEN];
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            char *line = strtok(buffer, "\n");
            while (line) {
                if (command_count < MAX_COMMANDS) {
                    commands[command_count++] = strdup(line);
                }
                line = strtok(NULL, "\n");
            }
        }
        close(fd);
        if (command_count > 0) {
            last_command = commands[command_count - 1];
        }
    }

    if (last_command && strcmp(command, last_command) == 0) {
        for (int i = 0; i < command_count; i++) {
            free(commands[i]);
        }
        return;
    }

    fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return;
    }

    int saved = dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);

    for (int i = 0; i < command_count; i++) {
        printf("%s\n", commands[i]);
        free(commands[i]);
    }
    printf("%s\n", command);

    dup2(saved, STDOUT_FILENO);
    close(saved);
    close(fd);
}

void display_log(const char *home) {
    char file_path[MAX_PATH_LEN];
    snprintf(file_path, sizeof(file_path), "%s/newfile.txt", home);

    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        return;
    }

    char buffer[MAX_LINE_LEN];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }
    close(fd);
}

void purge_log(const char *home) {
    char file_path[MAX_PATH_LEN];
    snprintf(file_path, sizeof(file_path), "%s/newfile.txt", home);

    int fd = open(file_path, O_WRONLY | O_TRUNC);
    if (fd >= 0) {
        close(fd);
    }
}

void print_command_from_log(const char *home, int index) {
    char file_path[MAX_PATH_LEN];
    snprintf(file_path, sizeof(file_path), "%s/newfile.txt", home);

    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        return;
    }

    char buffer[MAX_LINE_LEN];
    ssize_t bytes_read;
    char *commands[15];
    int command_count = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        char *line = strtok(buffer, "\n");
        while (line) {
            if (command_count < 15) {
                commands[command_count++] = strdup(line);
            }
            line = strtok(NULL, "\n");
        }
    }
    close(fd);

    int actual = command_count - 1 - index + 1;
    if (actual < 0 || actual >= command_count) {
        fprintf(stderr, "Invalid index\n");
        return;
    }

    char *cmd = strdup(commands[actual]);
    cmd[strcspn(cmd, "\n")] = '\0';
    and_input(cmd);

    for (int i = 0; i < command_count; i++) {
        free(commands[i]);
    }
}
