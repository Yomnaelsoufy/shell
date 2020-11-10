#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
FILE *fptr;
int res;
char *buffer;
//function of other commands
void doCommand(char *buffer){
pid_t child_id;
int status;
int ampersand=0;
int i=0;
//to know if user want to execute the command in background
//at end of command '&'
for(i=1;buffer[i]!='\0';i++);

if(buffer[i-1]=='&')
 {
memmove(&buffer[i-1],&buffer[i],strlen(buffer)-i+1);
 ampersand=1;
 }
//fork a child process
child_id=fork();
if(child_id<0){
//error occur
printf("Child can't occur!");
}
else if(child_id ==0){
//child process
char *p=strtok(buffer," ");
char *arr[3];
i=0;
while(p!=NULL)
{
arr[i++]=p;
p=strtok(NULL," ");
}
arr[i]=NULL;
res=execvp(arr[0],arr);
if(res==-1)printf("Command %s not Found.",buffer);
exit(-1);
}
else{
//wait for child to complete
if(ampersand==0){
waitpid(child_id,&status,0);
printf("wait for child complete.\n");
}}
}
//handle terminate from child
void* termination(int signal){
fputs("Child was terminated\n",fptr);
//printf("wait return %d\n",res);
}


int main()
{
fptr=fopen("logfile.txt","w");
if(fptr==NULL){
printf("Unable to create file.\n");
exit(EXIT_FAILURE);
}
size_t bufsize=32;
//termination setup
signal(SIGCHLD,termination);
    while(1){
    printf("shell> ");
    //take input******//
    buffer=(char *)malloc(bufsize*sizeof(char));
if(strcmp(buffer,"\n")==0)continue;
else if(buffer==NULL)
{
    perror("Unable to allocate buffer");
    exit(1);
}
getline(&buffer,&bufsize,stdin);
//***************************//
//shell command exit
if(strcmp(buffer,"exit\n")==0)
 exit(0);

//other commands (ls,..)
doCommand(strtok(buffer,"\n"));
    }fclose(fptr);
    return 0;
}
