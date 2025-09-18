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
struct Alias{char n[MAX_ALIAS_LENGTH];char c[MAX_COMMAND_LENGTH];};
struct Function{char n[MAX_FUNCTION_NAME];char b[MAX_FUNCTION_BODY];};
extern struct Alias aliases[MAX_ALIASES];extern struct Function functions[MAX_FUNCTIONS];
void load_myshrc();char*get_alias_command(char*n);void process_input(const char*i,char*o);
void execute_function(char*fn,char*arg);void mk_hop(char*c);void hop_seek(char*c);void handle_command(char*c,int fg);
#endif
