#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main() {
    char command[4096];
    while (1) {
        printf("$ ");
        if (fgets(command, sizeof(command), stdin) == NULL) break;
        command[strcspn(command, "\n")] = '\0';
        if (strcmp(command, "quit") == 0) break;
        printf("Command: %s\n", command);
    }
    return 0;
}
