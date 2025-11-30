#ifndef FGBG_H
#define FGBG_H

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include "keyboard.h"
#include "spec6.h"

void process_fg_command(const char *command);
void process_bg_command(const char *command);

#endif
