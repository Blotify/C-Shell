#ifndef NEONATE_H
#define NEONATE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <sys/select.h>

void process_neonate_command(const char *command);
int is_x_key_pressed();
pid_t get_most_recent_pid();
long long get_process_start_time(pid_t pid);

#endif
