#ifndef REVEAL_H
#define REVEAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "hop.h"

void list_directory_contents(const char *path, int show_hidden, int show_long);
void process_reveal_command(char *command);

#endif
