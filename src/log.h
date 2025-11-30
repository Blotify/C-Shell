#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "proc.h"
#include "hop.h"

#define MAX_COMMANDS 14
#define MAX_PATH_LEN 1024
#define MAX_LINE_LEN 1024

void add_to_log(const char *command, const char *home);
void display_log(const char *home);
void purge_log(const char *home);
void print_command_from_log(const char *home, int index);

#endif
