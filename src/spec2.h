#ifndef SPEC2_H
#define SPEC2_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hop.h"
#include "prompt.h"
char* trim_whitespace(char* str);
int count_occurrences(const char *str, char ch);
void execute_input(char* command, int flag);
void semicolon_input(char* command, int flag);
void and_input(char* command);
#endif
