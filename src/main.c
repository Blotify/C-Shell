#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "prompt.h"
#include "spec2.h"
int main() {
    char command[4096];
    while (1) {
        update_prompt();
        if (fgets(command, sizeof(command), stdin) == NULL) break;
        command[strcspn(command, "\n")] = '\0';
        if (strcmp(command, "quit") == 0) break;
        and_input(command);
    }
    return 0;
}
