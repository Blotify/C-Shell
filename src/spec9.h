#ifndef SPEC9_H
#define SPEC9_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include "spec2.h"
#include "spec6.h"

#define MAX_ALIASES 100
#define MAX_FUNCTIONS 20
#define MAX_ALIAS_LENGTH 100
#define MAX_COMMAND_LENGTH 1000
#define MAX_FUNCTION_NAME 50
#define MAX_FUNCTION_BODY 1000

struct Alias {
    char name[MAX_ALIAS_LENGTH];
    char command[MAX_COMMAND_LENGTH];
};

struct Function {
    char name[MAX_FUNCTION_NAME];
    char body[MAX_FUNCTION_BODY];
};

extern struct Alias aliases[MAX_ALIASES];
extern struct Function functions[MAX_FUNCTIONS];

void load_myshrc();
char *get_alias_command(char *name);
void process_input(const char *input, char *output);
void execute_function(char *function_name, char *arg);
void mk_hop(char *command);
void hop_seek(char *command);
void handle_command(char *command, int fg);

#endif
