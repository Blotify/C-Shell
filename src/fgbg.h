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
void pfg(const char*c);void pbg(const char*c);
#endif
