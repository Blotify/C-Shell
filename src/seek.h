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
void seek_in_directory(const char* s, const char* d, int fd, int ff, int fe);
void expand_path(char* p); void search_directory(const char* d, const char* s, int fd, int ff, int fe, int* f);
#endif
