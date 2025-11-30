#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include "hop.h"
#include "prompt.h"
#include "proc.h"
#include "log.h"
#include "spec2.h"
#include "spec6.h"
#include "reveal.h"
#include "seek.h"
#include "activities.h"
#include "keyboard.h"
#include "ior.h"
#include "iman.h"
#include "neonate.h"
#include "fgbg.h"
#include "spec9.h"

pid_t shell_pid;

int main() {
    shell_pid = getpid();
    setup_keyboard_handlers();
    signal(SIGCHLD, handle_sigchld);
    char command[4096];

    initialize_directories();

    while (1) {
        update_prompt();

        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        int flag = 0;
        if (strstr(command, "log ") || strstr(command, "log;") || strstr(command, "log&") || strstr(command, "log\n")) {
            flag = 1;
        }

        command[strcspn(command, "\n")] = '\0';

        if (strstr(command, "quit")) {
            return 0;
        }

        if (!flag) {
            add_to_log(command, home_dir);
        }

        and_input(command);
    }

    return 0;
}
