#include "spec2.h"
char* trim_whitespace(char* str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = '\0';
    return str;
}
int count_occurrences(const char *str, char ch) {
    int count = 0;
    while (*str) { if (*str == ch) count++; str++; }
    return count;
}
void execute_input(char* command, int flag) {
    if (strlen(command) < 1) return;
    printf("Executing: %s\n", command);
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
