#include "prompt.h"

void update_prompt() {
    char cwd[1024];
    char *username = getlogin();
    char systemname[HOST_NAME_MAX];

    gethostname(systemname, sizeof(systemname));

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        char display_path[1024];
        if (strstr(cwd, home_dir) == cwd) {
            snprintf(display_path, sizeof(display_path), "~%s", cwd + strlen(home_dir));
        } else {
            strncpy(display_path, cwd, sizeof(display_path));
        }
        printf(PROMPT_COLOR "<%s@%s:%s> " RESET_COLOR, username, systemname, display_path);
    } else {
        perror("getcwd");
    }
}
