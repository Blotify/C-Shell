#include "ping.h"
void ppc(char*c){char*t,*r=c;strtok_r(r," ",&r);t=strtok_r(r," ",&r);if(!t){fprintf(stderr,"Invalid ping\n");return;}pid_t p=atoi(t);t=strtok_r(r," ",&r);if(!t){fprintf(stderr,"Invalid ping\n");return;}int sn=atoi(t)%32;if(kill(p,0)==-1&&errno==ESRCH){fprintf(stderr,"No such process\n");return;}if(kill(p,sn)==0)printf("Sent signal %d to pid %d\n",sn,p);else perror("signal");}
