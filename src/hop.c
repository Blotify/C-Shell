#include "hop.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

char home_dir[1024];
char previous_dir[1024];

void initialize_directories() {
    if (getcwd(home_dir, sizeof(home_dir)) == NULL) {
        perror("getcwd");
        return;
    }
    previous_dir[0] = '\0';
}

void hop_to_directory(const char *path) {
    char cwd[1024];
    char temp_dir[1024];
    char resolved_path[1024];

    if (getcwd(temp_dir, sizeof(temp_dir)) == NULL) {
        perror("getcwd");
        return;
    }

    if (path[0] == '~') {
        snprintf(resolved_path, sizeof(resolved_path), "%s%s", home_dir, path + 1);
        path = resolved_path;
    } else if (strcmp(path, "-") == 0) {
        if (previous_dir[0] == '\0') {
            fprintf(stderr, "hop: no previous directory\n");
            return;
        }
        path = previous_dir;
    }

    if (chdir(path) != 0) {
        perror("hop");
        return;
    }

    strcpy(previous_dir, temp_dir);

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd");
    }
}

void process_hop_command(char *command) {
    char *token = strtok(command, " ");
    token = strtok(NULL, " ");

    if (token == NULL) {
        hop_to_directory("~");
        return;
    }

    while (token != NULL) {
        hop_to_directory(token);
        token = strtok(NULL, " ");
    }
}
