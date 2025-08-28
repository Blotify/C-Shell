#include "reveal.h"
int compare_entries(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}
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
    if (!dir) { perror("opendir"); return; }
    struct dirent* entry; struct stat file_stat;
    char fullpath[2048]; char* entries[1024]; int entry_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') continue;
        entries[entry_count] = strdup(entry->d_name); entry_count++;
    }
    closedir(dir);
    qsort(entries, entry_count, sizeof(char*), compare_entries);
    for (int i = 0; i < entry_count; i++) {
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entries[i]);
        if (stat(fullpath, &file_stat) == -1) { free(entries[i]); continue; }
        if (S_ISDIR(file_stat.st_mode)) printf("\033[1;34m%s\033[0m", entries[i]);
        else if (file_stat.st_mode & S_IXUSR) printf("\033[1;32m%s\033[0m", entries[i]);
        else printf("\033[1;37m%s\033[0m", entries[i]);
        if (long_format) printf("  Size: %lld", (long long)file_stat.st_size);
        printf("\n"); free(entries[i]);
    }
}
