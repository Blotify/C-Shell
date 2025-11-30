#include "reveal.h"

int check_if_command_ends_with_dash(const char *command) {
    char temp[256];
    strncpy(temp, command, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    char *end = temp + strlen(temp) - 1;
    while (end > temp && (*end == ' ' || *end == '\t')) {
        end--;
    }
    return (end >= temp && *end == '-') ? 1 : 0;
}

int compare_entries(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void list_directory_contents(const char *path, int show_hidden, int show_long) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    struct stat file_stat;
    char full_path[2048];
    char *entries[1024];
    int entry_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue;
        }
        entries[entry_count] = strdup(entry->d_name);
        entry_count++;
    }
    closedir(dir);

    qsort(entries, entry_count, sizeof(char *), compare_entries);

    for (int i = 0; i < entry_count; i++) {
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entries[i]);
        if (stat(full_path, &file_stat) == -1) {
            free(entries[i]);
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            printf("\033[1;34m%s\033[0m", entries[i]);
        } else if (file_stat.st_mode & S_IXUSR) {
            printf("\033[1;32m%s\033[0m", entries[i]);
        } else {
            printf("\033[1;37m%s\033[0m", entries[i]);
        }

        if (show_long) {
            printf("  Size: %lld", (long long)file_stat.st_size);
        }
        printf("\n");
        free(entries[i]);
    }
}

void process_reveal_command(char *command) {
    int show_all = 0;
    int show_long = 0;
    int is_previous = 0;
    char path[1024] = ".";

    if (check_if_command_ends_with_dash(command)) {
        is_previous = 1;
    }

    char *token = strtok(command, " ");
    token = strtok(NULL, " ");

    while (token) {
        if (token[0] == '-') {
            for (int i = 1; token[i]; i++) {
                if (token[i] == 'a') show_all = 1;
                if (token[i] == 'l') show_long = 1;
            }
        } else {
            strncpy(path, token, 1024);
        }
        token = strtok(NULL, " ");
    }

    if (strcmp(path, "~") == 0) {
        strncpy(path, home_dir, 1024);
    }

    if (is_previous) {
        strncpy(path, previous_dir, 1024);
    }

    list_directory_contents(path, show_all, show_long);
}
