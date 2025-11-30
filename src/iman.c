#include "iman.h"

void get_command_word(const char *input, char *cmd, size_t size) {
    const char *space = strchr(input, ' ');
    if (space) {
        space++;
        const char *second_space = strchr(space, ' ');
        if (second_space) {
            size_t len = second_space - space;
            if (len >= size) {
                len = size - 1;
            }
            strncpy(cmd, space, len);
            cmd[len] = '\0';
        } else {
            strncpy(cmd, space, size - 1);
            cmd[size - 1] = '\0';
        }
    } else {
        cmd[0] = '\0';
    }
}

void strip_html_tags(char *input, char *output) {
    int in_tag = 0;
    size_t j = 0;
    for (size_t i = 0; input[i]; i++) {
        if (input[i] == '<') {
            in_tag = 1;
        } else if (input[i] == '>') {
            in_tag = 0;
        } else if (!in_tag) {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}

void url_encode(const char *src, char *dest, size_t size) {
    const char *hex = "0123456789ABCDEF";
    size_t pos = 0;
    for (size_t i = 0; src[i] && pos < size - 1; i++) {
        if (isalnum((unsigned char)src[i]) || src[i] == '-' || src[i] == '_' || src[i] == '.' || src[i] == '~') {
            dest[pos++] = src[i];
        } else if (pos + 3 < size) {
            dest[pos++] = '%';
            dest[pos++] = hex[(unsigned char)src[i] >> 4];
            dest[pos++] = hex[(unsigned char)src[i] & 15];
        }
    }
    dest[pos] = '\0';
}

void send_request(int sock, const char *command) {
    char request[BUFFER_SIZE];
    char encoded[BUFFER_SIZE];
    url_encode(command, encoded, sizeof(encoded));
    snprintf(request, sizeof(request) + 100,
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: " HOST "\r\n"
             "Connection: close\r\n\r\n",
             encoded);
    send(sock, request, strlen(request), 0);
}

void receive_and_print_response(int sock, const char *command) {
    char buffer[BUFFER_SIZE];
    char clean_line[BUFFER_SIZE * 10];
    char answer[BUFFER_SIZE * 20] = "";
    ssize_t bytes_read;
    int in_html = 0;

    while ((bytes_read = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_read] = '\0';
        if (!in_html) {
            char *html_start = strstr(buffer, "<html>");
            if (html_start) {
                in_html = 1;
                strip_html_tags(html_start, clean_line);
                strncat(answer, clean_line, sizeof(answer) - strlen(answer) - 1);
            }
        } else {
            strip_html_tags(buffer, clean_line);
            strncat(answer, clean_line, sizeof(answer) - strlen(answer) - 1);
        }
    }

    printf("%s", answer);
}

void process_iman_command(const char *input) {
    char cmd[256];
    get_command_word(input, cmd, sizeof(cmd));

    if (strlen(cmd) == 0) {
        printf("Usage: iMan <cmd>\n");
        return;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(HOST, PORT, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        close(sock);
        return;
    }

    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        perror("connect");
        close(sock);
        return;
    }

    freeaddrinfo(res);

    send_request(sock, cmd);
    receive_and_print_response(sock, cmd);
    close(sock);
}
