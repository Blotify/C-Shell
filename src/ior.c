#include "ior.h"

int file_exists(const char *file) {
    return access(file, F_OK) == 0;
}

void redirect_input(const char *file) {
    int fd = open(file, O_RDONLY);
    if (fd < 0) {
        printf("No such input file found!\n");
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void redirect_output(const char *file, int append) {
    int fd;
    if (append) {
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    } else {
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    if (fd < 0) {
        perror("output");
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

char *dup_command = NULL;

void handle_io_redirection(char *command, int fg) {
    char *input_file = NULL;
    char *output_file = NULL;
    int append = 0;

    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);

    char *input_redir;
    int first_flag = 0;

    if (strstr(command, ">") && strstr(command, "<")) {
        dup_command = strdup(command);
        input_redir = strchr(dup_command, '<');
        first_flag = 1;
    } else {
        input_redir = strchr(command, '<');
    }

    if (input_redir) {
        *input_redir = '\0';
        input_file = strtok(input_redir + 1, " ");
        if (!file_exists(input_file)) {
            printf("No such input file found!\n");
            dup2(original_stdin, STDIN_FILENO);
            dup2(original_stdout, STDOUT_FILENO);
            close(original_stdin);
            close(original_stdout);
            return;
        }
    }

    char *output_redir = strchr(command, '>');
    if (output_redir) {
        if (*(output_redir + 1) == '>') {
            append = 1;
            *output_redir = '\0';
            output_file = strtok(output_redir + 2, " ");
        } else {
            *output_redir = '\0';
            output_file = strtok(output_redir + 1, " ");
        }
    }

    if (input_file) {
        redirect_input(input_file);
    }
    if (output_file) {
        redirect_output(output_file, append);
    }

    if (!first_flag) {
        if (command && strlen(command) > 0) {
            execute_post_input(command, fg);
        }
    } else {
        if (dup_command && strlen(dup_command) > 0) {
            execute_post_input(dup_command, fg);
        }
    }

    fflush(stdout);
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}

int is_valid_command(const char *command) {
    char *str = strdup(command);
    if (!str) {
        return 0;
    }
    int valid = 1;
    int len = strlen(str);
    for (int i = 0; i < len - 1; i++) {
        if (str[i] == '|' && str[i + 1] == '|') {
            valid = 0;
            break;
        }
    }
    free(str);
    return valid;
}

void handle_pipes(char *command, int fg) {
    char *commands[100];
    int command_count = 0;

    if (!is_valid_command(command)) {
        fprintf(stderr, "Invalid\n");
        return;
    }

    if (command[0] == '|' || command[strlen(command) - 1] == '|') {
        fprintf(stderr, "Invalid use of pipe\n");
        return;
    }

    char *token = strtok(command, "|");
    while (token && command_count < 100) {
        while (*token == ' ') {
            token++;
        }
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ') {
            end--;
        }
        *(end + 1) = '\0';

        if (strlen(token) == 0) {
            fprintf(stderr, "Invalid use of pipe\n");
            return;
        }

        commands[command_count++] = token;
        token = strtok(NULL, "|");
    }

    if (command_count == 1) {
        handle_io_redirection(commands[0], fg);
        return;
    }

    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    int pipes[99][2];

    for (int i = 0; i < command_count - 1; i++) {
        pipe(pipes[i]);
    }

    for (int i = 0; i < command_count; i++) {
        if (i > 0) {
            dup2(pipes[i - 1][0], STDIN_FILENO);
            close(pipes[i - 1][0]);
        }
        if (i < command_count - 1) {
            dup2(pipes[i][1], STDOUT_FILENO);
            close(pipes[i][1]);
        }

        if (i != command_count - 1) {
            handle_io_redirection(commands[i], 0);
        } else {
            handle_io_redirection(commands[i], fg);
        }

        if (i > 0) {
            dup2(original_stdin, STDIN_FILENO);
        }
        if (i < command_count - 1) {
            dup2(original_stdout, STDOUT_FILENO);
        }
    }

    for (int i = 0; i < command_count - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}
