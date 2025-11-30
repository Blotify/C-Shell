#include "seek.h"
#include "hop.h"

void expand_path(char *path) {
    if (path[0] == '~') {
        char temp[MAX_PATH];
        snprintf(temp, MAX_PATH, "%s%s", home_dir, path + 1);
        strncpy(path, temp, MAX_PATH);
    } else if (strcmp(path, "-") == 0) {
        strncpy(path, previous_dir, MAX_PATH);
    }
}

void print_match(const char *path, struct stat *st) {
    if (S_ISDIR(st->st_mode)) {
        printf("\033[1;34m%s\033[0m\n", path);
    } else {
        printf("\033[1;32m%s\033[0m\n", path);
    }
}

void handle_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        return;
    }
    int c;
    while ((c = fgetc(f)) != EOF) {
        putchar(c);
    }
    fclose(f);
}

void seek_in_directory(const char *search, const char *dir, int fd, int ff, int fe) {
    int found = 0;
    if (fd && ff) {
        fprintf(stderr, "Invalid flags!\n");
        return;
    }
    search_directory(dir, search, fd, ff, fe, &found);
    if (!found) {
        printf("No match found!\n");
    }
}

void search_directory(const char *dir, const char *search, int fd, int ff, int fe, int *found) {
    DIR *d = opendir(dir);
    if (!d) {
        return;
    }

    struct dirent *entry;
    struct stat st;
    char full_path[MAX_PATH];
    int match_count = 0;
    char match_path[MAX_PATH];
    int is_dir = 0;

    while ((entry = readdir(d)) != NULL) {
        snprintf(full_path, MAX_PATH, "%s/%s", dir, entry->d_name);
        if (stat(full_path, &st) == -1) {
            continue;
        }

        int is_directory = S_ISDIR(st.st_mode);
        if ((fd && !is_directory) || (ff && is_directory)) {
            continue;
        }

        if (strcmp(entry->d_name, search) == 0) {
            print_match(full_path, &st);
            *found = 1;
            match_count++;
            if (match_count == 1) {
                strncpy(match_path, full_path, MAX_PATH);
                is_dir = is_directory;
            }
        }

        if (is_directory && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            search_directory(full_path, search, fd, ff, fe, found);
        }
    }
    closedir(d);

    if (fe && match_count == 1) {
        if (is_dir) {
            if (access(match_path, X_OK) == 0) {
                if (chdir(match_path) == 0) {
                    printf("New folder!\n");
                }
            } else {
                printf("Missing permissions!\n");
            }
        } else {
            if (access(match_path, R_OK) == 0) {
                handle_file(match_path);
            } else {
                printf("Missing permissions!\n");
            }
        }
    }
}
