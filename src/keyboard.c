#include "keyboard.h"
#include "spec6.h"
volatile pid_t fgp=0;
void hsi(int s){if(fgp>0){kill(fgp,SIGINT);printf("\nInterrupted %d\n",fgp);for(int i=0;i<MAX_PROCS;i++){if(processes[i].pid==fgp){processes[i].state=1;processes[i].end_time=time(NULL);break;}}fgp=0;}}
void hst(int s){if(fgp>0&&fgp!=shell_pid){kill(fgp,SIGTSTP);printf("\nStopped %d\n",fgp);for(int i=0;i<MAX_PROCS;i++){if(processes[i].pid==fgp){processes[i].state=2;break;}}setpgid(fgp,fgp);signal(SIGTTOU,SIG_IGN);tcsetpgrp(STDIN_FILENO,shell_pid);signal(SIGTTOU,SIG_DFL);fgp=0;}}
void hsq(int s){printf("\nLogging out...\n");for(int i=0;i<MAX_PROCS;i++)if(processes[i].pid!=0)kill(processes[i].pid,SIGKILL);exit(0);}
void skh(){struct sigaction sa,sa2,sa3;sa.sa_handler=hsi;sigfillset(&sa.sa_mask);sigaction(SIGINT,&sa,NULL);sa2.sa_handler=hst;sigemptyset(&sa2.sa_mask);sa2.sa_flags=SA_RESTART;sigaction(SIGTSTP,&sa2,NULL);sa3.sa_handler=hsq;sigfillset(&sa3.sa_mask);sigaction(SIGQUIT,&sa3,NULL);}
void sfp(pid_t p){fgp=p;}
