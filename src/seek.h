#ifndef SEEK_H
#define SEEK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "hop.h"

#define MAX_PATH 4096

void seek_in_directory(const char *search, const char *dir, int fd, int ff, int fe);
void expand_path(char *path);
void search_directory(const char *dir, const char *search, int fd, int ff, int fe, int *found);
void print_match(const char *path, struct stat *st);
void handle_file(const char *path);

#endif
