#include "prompt.h"
void update_prompt() {
    char cwd[1024], *username = getlogin(), systemname[HOST_NAME_MAX];
    gethostname(systemname, sizeof(systemname));
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf(PROMPT_COLOR "<%s@%s:~> " RESET_COLOR, username, systemname);
    else perror("getcwd");
}
