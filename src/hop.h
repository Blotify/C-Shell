#ifndef HOP_H
#define HOP_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
extern char home_dir[1024];
extern char previous_dir[1024];
void initialize_directories();
void hop_to_directory(const char *path);
void process_hop_command(char *command);
#endif
