#include "spec9.h"

int alias_count = 0;
int function_count = 0;
int first_flag = 0;
struct Alias aliases[MAX_ALIASES];
struct Function functions[MAX_FUNCTIONS];

void load_myshrc() {
    FILE *f = fopen(".myshrc", "r");
    if (!f) {
        return;
    }

    char line[MAX_COMMAND_LENGTH];
    char current_function_name[MAX_FUNCTION_NAME] = "";
    char function_body[MAX_FUNCTION_BODY] = "";
    int in_function = 0;

    while (fgets(line, sizeof(line), f)) {
        trim_whitespace(line);
        if (line[0] == '#') {
            continue;
        }

        if (strncmp(line, "alias", 5) == 0) {
            char *name_start = line + 5;
            char *eq = strchr(name_start, '=');
            if (eq) {
                *eq = '\0';
                char *name = name_start;
                char *command = eq + 1;
                trim_whitespace(name);
                trim_whitespace(command);
                if (alias_count < MAX_ALIASES) {
                    strncpy(aliases[alias_count].name, name, MAX_ALIAS_LENGTH - 1);
                    strncpy(aliases[alias_count].command, command, MAX_COMMAND_LENGTH - 1);
                    alias_count++;
                }
            }
        } else if (strncmp(line, "func", 4) == 0) {
            char *func_start = line + 4;
            while (*func_start == ' ') {
                func_start++;
            }
            char *func_end = func_start;
            while (*func_end && *func_end != ' ' && *func_end != '(') {
                func_end++;
            }
            *func_end = '\0';
            strncpy(current_function_name, func_start, MAX_FUNCTION_NAME - 1);
            in_function = 1;
            function_body[0] = '\0';
        } else if (in_function) {
            if (strcmp(line, "}") == 0) {
                in_function = 0;
                if (function_count < MAX_FUNCTIONS) {
                    strncpy(functions[function_count].name, current_function_name, MAX_FUNCTION_NAME - 1);
                    strncpy(functions[function_count].body, function_body, MAX_FUNCTION_BODY - 1);
                    function_count++;
                }
            } else {
                char *comment_start = strchr(line, '#');
                if (comment_start) {
                    *comment_start = '\0';
                    trim_whitespace(line);
                }
                if (strlen(line) > 0) {
                    strcat(function_body, line);
                    strcat(function_body, "\n");
                }
            }
        }
    }
    fclose(f);
}

char *get_alias_command(char *name) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(trim_whitespace(aliases[i].name), name) == 0) {
            return aliases[i].command;
        }
    }
    return NULL;
}

void process_input(const char *input, char *output) {
    strncpy(output, input, MAX_COMMAND_LENGTH);
}

void execute_function(char *function_name, char *arg) {
    for (int i = 0; i < function_count; i++) {
        if (strcmp(trim_whitespace(functions[i].name), function_name) == 0) {
            char cmd[MAX_COMMAND_LENGTH];
            strcpy(cmd, trim_whitespace(functions[i].body));

            char *double_pipe = strstr(cmd, "$1");
            while (double_pipe) {
                memmove(double_pipe + strlen(arg), double_pipe + 2, strlen(double_pipe + 2) + 1);
                memcpy(double_pipe, arg, strlen(arg));
                double_pipe = strstr(double_pipe + strlen(arg), "$1");
            }

            char *open_brace = strchr(cmd, '{');
            char *close_brace = strrchr(cmd, '}');
            if (open_brace && close_brace) {
                memmove(cmd, open_brace + 1, close_brace - open_brace - 1);
                cmd[close_brace - open_brace - 1] = '\0';
            }

            trim_whitespace(cmd);
            char new_command[4096];
            process_input(cmd, new_command);
            and_input(new_command);
            return;
        }
    }
    printf("Function %s not found.\n", function_name);
}

void mk_hop(char *command) {
    char dir[MAX_COMMAND_LENGTH];
    if (sscanf(command, "mk_hop %s", dir) == 1) {
        execute_function("mk_hop", dir);
    } else {
        printf("Usage: mk_hop <dir>\n");
    }
}

void hop_seek(char *command) {
    char dir[MAX_COMMAND_LENGTH];
    if (sscanf(command, "hop_seek %s", dir) == 1) {
        execute_function("hop_seek", dir);
    } else {
        printf("Usage: hop_seek <dir>\n");
    }
}

void handle_command(char *command, int fg) {
    if (!first_flag) {
        load_myshrc();
        first_flag = 1;
    }

    char first_word[MAX_ALIAS_LENGTH];
    sscanf(command, "%s", first_word);

    char *alias_cmd = get_alias_command(first_word);
    if (alias_cmd) {
        execute_post_input(trim_whitespace(alias_cmd), fg);
    } else {
        execute_post_input(command, fg);
    }
}
