#include "spec2.h"
#include "reveal.h"
#include "seek.h"
#include "ping.h"
#include "log.h"
#include "iman.h"
#include "neonate.h"
#include "fgbg.h"
#include "activities.h"
#include "spec9.h"
#include "ior.h"

char *trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == 0) {
        return str;
    }
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    return str;
}

int count_occurrences(const char *str, char c) {
    int count = 0;
    while (*str) {
        if (*str == c) {
            count++;
        }
        str++;
    }
    return count;
}

int processor(char *command, char *target) {
    char command_copy[4096];
    strncpy(command_copy, command, sizeof(command_copy));
    command_copy[sizeof(command_copy) - 1] = '\0';

    char *first = strtok(command_copy, " ");
    return (first && strcmp(first, target) == 0) ? 1 : 0;
}

void execute_post_input(char *command, int fg) {
    if (processor(command, "hop")) {
        process_hop_command(command);
        return;
    } else if (processor(command, "proclore")) {
        process_proclore_command(command);
        return;
    } else if (processor(command, "ping")) {
        process_ping_command(command);
        return;
    } else if (processor(command, "log")) {
        if (strcmp(command, "log purge") == 0) {
            purge_log(home_dir);
            printf("Log cleared.\n");
            return;
        } else if (strncmp(command, "log execute ", 12) == 0) {
            print_command_from_log(home_dir, atoi(command + 12));
            return;
        } else {
            display_log(home_dir);
            return;
        }
    } else if (processor(command, "reveal")) {
        process_reveal_command(command);
        return;
    } else if (processor(command, "iMan")) {
        process_iman_command(command);
        return;
    } else if (processor(command, "neonate")) {
        process_neonate_command(command);
        return;
    } else if (processor(command, "mk_hop")) {
        mk_hop(command);
        return;
    } else if (processor(command, "hop_seek")) {
        hop_seek(command);
        return;
    } else if (processor(command, "fg")) {
        process_fg_command(command);
        return;
    } else if (processor(command, "bg")) {
        process_bg_command(command);
        return;
    } else if (processor(command, "activities")) {
        activity_generator();
        return;
    } else if (processor(command, "seek")) {
        char *token = strtok(command, " ");
        token = strtok(NULL, " ");
        int d = 0, f = 0, e = 0;
        char search[MAX_PATH] = {0};
        char target_dir[MAX_PATH] = {0};

        if (token && token[0] == '-') {
            while (token && token[0] == '-') {
                for (int i = 1; token[i]; i++) {
                    if (token[i] == 'd') d = 1;
                    if (token[i] == 'f') f = 1;
                    if (token[i] == 'e') e = 1;
                }
                token = strtok(NULL, " ");
            }
        }

        if (d && f) {
            fprintf(stderr, "Invalid flags!\n");
            return;
        }

        if (token) {
            strncpy(search, token, MAX_PATH);
            token = strtok(NULL, " ");
        }

        if (token) {
            strncpy(target_dir, token, MAX_PATH);
            expand_path(target_dir);
        } else {
            strncpy(target_dir, ".", MAX_PATH);
        }

        expand_path(search);
        seek_in_directory(search, target_dir, d, f, e);
        return;
    } else {
        syscommands(command, fg);
    }
}

int is_invalid_sequence(const char *command) {
    int len = strlen(command);
    char prev = '\0';
    int greater_count = 0;

    for (int i = 0; i < len; i++) {
        if (isspace(command[i])) {
            continue;
        }
        if (command[i] == '<' || command[i] == '|' || command[i] == '>' || command[i] == '&') {
            if ((prev == '<' || prev == '|' || prev == '&') ||
                (command[i] == '>' && greater_count >= 2) ||
                (prev == '>' && command[i] == '&')) {
                return 1;
            }
            if (command[i] == '>') {
                greater_count++;
            } else {
                greater_count = 0;
            }
            prev = command[i];
        } else {
            prev = '\0';
            greater_count = 0;
        }
    }
    return 0;
}

void execute_input(char *command, int fg) {
    if (strlen(command) < 1 || strcmp(command, " ") == 0) {
        return;
    }

    if (strstr(command, ">") || strstr(command, "<") || strstr(command, ">>") || strstr(command, "|")) {
        handle_pipes(command, fg);
        return;
    }

    handle_command(command, fg);
}

void semicolon_input(char *command, int fg) {
    int num = count_occurrences(command, ';');
    if (num == 0) {
        execute_input(command, fg);
        return;
    }

    char **commands = calloc(num + 1, sizeof(char *));
    char *token = strtok(command, ";");
    int i = 0;

    while (token) {
        token = trim_whitespace(token);
        commands[i] = strdup(token);
        i++;
        token = strtok(NULL, ";");
    }

    for (int j = 0; j < i; j++) {
        if (commands[j]) {
            if (j == i - 1 && fg) {
                execute_input(commands[j], 1);
            } else {
                execute_input(commands[j], 0);
            }
            free(commands[j]);
        }
    }
    free(commands);
}

void and_input(char *command) {
    if (is_invalid_sequence(command)) {
        fprintf(stderr, "Invalid command\n");
        return;
    }

    int num = count_occurrences(command, '&');
    if (num == 0) {
        semicolon_input(command, 0);
        return;
    }

    char **commands = calloc(num + 1, sizeof(char *));
    char *token = strtok(command, "&");
    int i = 0;

    while (token) {
        token = trim_whitespace(token);
        commands[i] = strdup(token);
        i++;
        token = strtok(NULL, "&");
    }

    for (int j = 0; j < i; j++) {
        if (commands[j]) {
            if (i == 1 || j != i - 1) {
                semicolon_input(commands[j], 1);
            } else {
                semicolon_input(commands[j], 0);
            }
            free(commands[j]);
        }
    }
    free(commands);
}
