#include "activities.h"
int cmp_p(const void*a,const void*b){return strcmp(((ProcessInfo*)a)->name,((ProcessInfo*)b)->name);}
void act(){ProcessInfo*pi;int c;get_more_process_info(&pi,&c);if(c==0){printf("No processes.\n");return;}qsort(pi,c,sizeof(ProcessInfo),cmp_p);int bf=0;for(int i=0;i<c;i++){const char*s;switch(pi[i].state){case 0:s="Running";break;case 1:s="Terminated";break;case 2:s="Stopped";break;default:s="Unknown";}if(pi[i].state!=1){bf=1;printf("%d : %s - %s\n",pi[i].pid,pi[i].name,s);}}if(!bf)printf("No processes.\n");free(pi);cleanup_finished_processes();}
