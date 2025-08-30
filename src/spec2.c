#include "spec2.h"
char* trim_whitespace(char* str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = '\0'; return str;
}
int count_occurrences(const char *str, char ch) {
    int count = 0;
    while (*str) { if (*str == ch) count++; str++; }
    return count;
}
int processor(char* command, char* totest) {
    char command_copy[4096];
    strncpy(command_copy, command, sizeof(command_copy));
    command_copy[sizeof(command_copy)-1] = '\0';
    char *first_word = strtok(command_copy, " ");
    return (first_word && strcmp(first_word, totest) == 0) ? 1 : 0;
}
void execute_post_input(char* command, int flag) {
    if (processor(command, "hop")) { process_hop_command(command); return; }
    else if (processor(command, "proclore")) { process_proclore_command(command); return; }
    else if (processor(command, "log")) {
        if (strcmp(command, "log purge") == 0) { purge_log(home_dir); printf("Log cleared.\n"); return; }
        else if (strncmp(command, "log execute ", 12) == 0) { print_command_from_log(home_dir, atoi(command+12)); return; }
        else { display_log(home_dir); return; }
    }
    else if (processor(command, "reveal")) { process_reveal_command(command); return; }
    else { printf("Unknown: %s\n", command); }
}
void execute_input(char* command, int flag) {
    if (strlen(command) < 1) return;
    execute_post_input(command, flag);
}
void semicolon_input(char* command, int flag) {
    int numsc = count_occurrences(command, ';');
    if (numsc == 0) { execute_input(command, flag); return; }
    char* token = strtok(command, ";");
    while (token != NULL) {
        token = trim_whitespace(token);
        if (token && strlen(token) > 0) execute_input(token, 0);
        token = strtok(NULL, ";");
    }
}
void and_input(char* command) {
    int numand = count_occurrences(command, '&');
    if (numand == 0) { semicolon_input(command, 0); return; }
    char* token = strtok(command, "&");
    while (token != NULL) {
        token = trim_whitespace(token);
        if (token && strlen(token) > 0) semicolon_input(token, 1);
        token = strtok(NULL, "&");
    }
}
