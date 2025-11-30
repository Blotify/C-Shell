#ifndef PROC_H
#define PROC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

void get_process_info(pid_t pid);
void process_proclore_command(char *command);

#endif
