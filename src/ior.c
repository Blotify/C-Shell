#include "ior.h"
int fex(const char*f){return access(f,F_OK)==0;}
void ri(const char*f){int fd=open(f,O_RDONLY);if(fd<0){printf("No such input file found!\n");exit(1);}dup2(fd,STDIN_FILENO);close(fd);}
void ro(const char*f,int a){int fd=a?open(f,O_WRONLY|O_CREAT|O_APPEND,0644):open(f,O_WRONLY|O_CREAT|O_TRUNC,0644);if(fd<0){perror("output");exit(1);}dup2(fd,STDOUT_FILENO);close(fd);}
void hio(char*c,int fg){char*inf=NULL,*outf=NULL;int a=0;int oi=dup(STDIN_FILENO),oo=dup(STDOUT_FILENO);char*ir=strchr(c,'<');if(ir){*ir='\0';inf=strtok(ir+1," ");if(!fex(inf)){printf("No such input file found!\n");dup2(oi,STDIN_FILENO);dup2(oo,STDOUT_FILENO);close(oi);close(oo);return;}}char*or=strchr(c,'>');if(or){if(*(or+1)=='>'){a=1;*or='\0';outf=strtok(or+2," ");}else{*or='\0';outf=strtok(or+1," ");}}if(inf)ri(inf);if(outf)ro(outf,a);if(c&&strlen(c)>0)epi(c,fg);fflush(stdout);dup2(oi,STDIN_FILENO);dup2(oo,STDOUT_FILENO);close(oi);close(oo);}
