#ifndef PROMPT_H
#define PROMPT_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include "hop.h"
#define HOST_NAME_MAX 4096
#define RESET_COLOR "\033[0m"
#define PROMPT_COLOR "\033[1;32m"
void update_prompt();
#endif
