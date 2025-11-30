#ifndef IMAN_H
#define IMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 4096
#define HOST "man.he.net"
#define PORT "80"

void get_command_word(const char *input, char *cmd, size_t size);
void strip_html_tags(char *input, char *output);
void url_encode(const char *src, char *dest, size_t size);
void send_request(int sock, const char *command);
void receive_and_print_response(int sock, const char *command);
void process_iman_command(const char *input);

#endif
