#include "reveal.h"
void process_reveal_command(char* command) {
    int show_all = 0, long_format = 0;
    char path[1024] = ".";
    char* token = strtok(command, " ");
    token = strtok(NULL, " ");
    while (token != NULL) {
        if (token[0] == '-') {
            for (int i = 1; token[i] != '\0'; i++) {
                if (token[i] == 'a') show_all = 1;
                else if (token[i] == 'l') long_format = 1;
            }
        } else { strncpy(path, token, 1024); }
        token = strtok(NULL, " ");
    }
    if (strcmp(path, "~") == 0) strncpy(path, home_dir, 1024);
    DIR* dir = opendir(path);
    if (!dir) { fprintf(stderr, "ERROR: Cannot access '%s'\n", path); return; }
    struct dirent *entry;
    struct stat file_stat;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') continue;
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        stat(full_path, &file_stat);
        if (S_ISDIR(file_stat.st_mode)) printf("\033[1;34m%s\033[0m\n", entry->d_name);
        else if (file_stat.st_mode & S_IXUSR) printf("\033[1;32m%s\033[0m\n", entry->d_name);
        else printf("%s\n", entry->d_name);
    }
    closedir(dir);
}
