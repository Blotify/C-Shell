#include "spec2.h"
char*t_(char*s){char*e;while(isspace((unsigned char)*s))s++;if(*s==0)return s;e=s+strlen(s)-1;while(e>s&&isspace((unsigned char)*e))e--;*(e+1)='\0';return s;}
int c_(const char*s,char c){int n=0;while(*s){if(*s==c)n++;s++;}return n;}
int p_(char*c,char*t){char cc[4096];strncpy(cc,c,sizeof(cc));cc[sizeof(cc)-1]='\0';char*f=strtok(cc," ");return f&&strcmp(f,t)==0?1:0;}
void epi(char*c,int fg){
if(p_(c,"hop")){process_hop_command(c);return;}
else if(p_(c,"proclore")){process_proclore_command(c);return;}
else if(p_(c,"ping")){ppc(c);return;}
else if(p_(c,"log")){if(strcmp(c,"log purge")==0){purge_log(home_dir);printf("Log cleared.\n");return;}else if(strncmp(c,"log execute ",12)==0){print_command_from_log(home_dir,atoi(c+12));return;}else{display_log(home_dir);return;}}
else if(p_(c,"reveal")){process_reveal_command(c);return;}
else if(p_(c,"iMan")){pic(c);return;}
else if(p_(c,"neonate")){pnc(c);return;}
else if(p_(c,"mk_hop")){mk_hop(c);return;}
else if(p_(c,"hop_seek")){hop_seek(c);return;}
else if(p_(c,"fg")){pfg(c);return;}
else if(p_(c,"bg")){pbg(c);return;}
else if(p_(c,"activities")){act();return;}
else if(p_(c,"seek")){char*t=strtok(c," ");t=strtok(NULL," ");int d=0,f=0,e=0;char s[MAX_PATH]={0},td[MAX_PATH]={0};if(t&&t[0]=='-'){while(t&&t[0]=='-'){for(int i=1;t[i];i++){if(t[i]=='d')d=1;if(t[i]=='f')f=1;if(t[i]=='e')e=1;}t=strtok(NULL," ");}}if(d&&f){fprintf(stderr,"Invalid flags!\n");return;}if(t){strncpy(s,t,MAX_PATH);t=strtok(NULL," ");}if(t){strncpy(td,t,MAX_PATH);expand_path(td);}else strncpy(td,".",MAX_PATH);expand_path(s);seek_in_directory(s,td,d,f,e);return;}
else{syscommands(c,fg);}}
int inv(const char*c){int l=strlen(c);char p='\0';int gc=0;for(int i=0;i<l;i++){if(isspace(c[i]))continue;if(c[i]=='<'||c[i]=='|'||c[i]=='>'||c[i]=='&'){if((p=='<'||p=='|'||p=='&')||(c[i]=='>'&&gc>=2)||(p=='>'&&c[i]=='&'))return 1;if(c[i]=='>')gc++;else gc=0;p=c[i];}else{p='\0';gc=0;}}return 0;}
void ei(char*c,int fg){if(strlen(c)<1||strcmp(c," ")==0)return;if(strstr(c,">")||strstr(c,"<")||strstr(c,">>")||strstr(c,"|")){hp(c,fg);return;}handle_command(c,fg);}
void si(char*c,int fg){int n=c_(c,';');if(n==0){ei(c,fg);return;}char**cm=calloc(n+1,sizeof(char*));char*t=strtok(c,";");int i=0;while(t){t=t_(t);cm[i]=strdup(t);i++;t=strtok(NULL,";");}for(int j=0;j<i;j++){if(cm[j]){if(j==i-1&&fg)ei(cm[j],1);else ei(cm[j],0);free(cm[j]);}}free(cm);}
void ai(char*c){if(inv(c)){fprintf(stderr,"Invalid command\n");return;}int n=c_(c,'&');if(n==0){si(c,0);return;}char**cm=calloc(n+1,sizeof(char*));char*t=strtok(c,"&");int i=0;while(t){t=t_(t);cm[i]=strdup(t);i++;t=strtok(NULL,"&");}for(int j=0;j<i;j++){if(cm[j]){if(i==1||j!=i-1)si(cm[j],1);else si(cm[j],0);free(cm[j]);}}free(cm);}
