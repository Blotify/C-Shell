#ifndef SPEC6_H
#define SPEC6_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include "hop.h"
#include "prompt.h"
#include "proc.h"
#include "log.h"
#define MAX_PROCS 1000
typedef struct {
    char* name; pid_t pid; int state; time_t end_time;
} ProcessInfo;
extern ProcessInfo processes[MAX_PROCS];
void handle_sigchld(int sig);
void time_monitor(struct timeval start, struct timeval end, char* command);
void syscommands(char* command, int flag);
int add_process(pid_t pid, const char* name, int is_background);
#endif
