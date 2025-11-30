#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

extern pid_t shell_pid;

void handle_sigint(int sig);
void handle_sigtstp(int sig);
void handle_sigquit(int sig);
void setup_keyboard_handlers();
void set_foreground_pid(pid_t pid);

#endif
