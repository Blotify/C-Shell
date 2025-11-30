#ifndef IOR_H
#define IOR_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "spec6.h"
#include "spec2.h"

int file_exists(const char *file);
void redirect_input(const char *file);
void redirect_output(const char *file, int append);
void handle_io_redirection(char *command, int fg);
int is_valid_command(const char *command);
void handle_pipes(char *command, int fg);

#endif
