#ifndef SPEC2_H
#define SPEC2_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hop.h"
#include "prompt.h"

char *trim_whitespace(char *str);
int count_occurrences(const char *str, char ch);
int processor(char *command, char *target);
void execute_post_input(char *command, int flag);
int is_invalid_sequence(const char *command);
void execute_input(char *command, int flag);
void semicolon_input(char *command, int flag);
void and_input(char *command);

#endif
