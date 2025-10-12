#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#include "spec9.h"
pid_t shell_pid;
int main(){shell_pid=getpid();skh();signal(SIGCHLD,handle_sigchld);char cmd[4096];initialize_directories();while(1){update_prompt();if(!fgets(cmd,sizeof(cmd),stdin))break;int fg=0;if(strstr(cmd,"log ")||strstr(cmd,"log;")||strstr(cmd,"log&")||strstr(cmd,"log\n"))fg=1;cmd[strcspn(cmd,"\n")]='\0';if(strstr(cmd,"quit"))return 0;if(!fg)add_to_log(cmd,home_dir);ai(cmd);}return 0;}
