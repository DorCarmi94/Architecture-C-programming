#include <linux/limits.h>

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include "LineParser.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


#define MAX_LINE 2048
void makeError();
void execute(cmdLine *pCmdLine, int dMode);


int main(int argc, char **argv) {

    int debugMode=0;
    int j;
    for (j = 1; j < argc; j++)
    {
        if(strlen(argv[j])>1 && argv[j][0]=='-'&& argv[j][1]=='D')
        {
            debugMode=1;
        }
    }
    
    char currentPath[PATH_MAX];
    char *pathPtr=currentPath;
    char lineFromTheUser[MAX_LINE];
    cmdLine *nextCommand;
    
    while (1)
    {
        getcwd(currentPath,PATH_MAX);
        printf("%s/",pathPtr);
        fgets(lineFromTheUser,MAX_LINE,stdin);
        if(strcmp(lineFromTheUser,"quit\n")==0)
        {
            exit(0);
        }
        
        nextCommand=parseCmdLines(lineFromTheUser);
        if(strcmp(nextCommand->arguments[0],"cd")==0)
        {
            int ans=chdir(nextCommand->arguments[1]);
            if(ans==-1)
            {
                perror("chdir: ");
            }
        }
        execute(nextCommand, debugMode);
    }

    /* char *bufprt=buf;
    // getcwd(bufprt,1000);
    // int x=0;
    // int y=5;
    // fopen("bla","r");

    // perror("Error");

    // printf("Hello world\n%s\n",buf);
     return 0;*/
}

void execute(cmdLine *pCmdLine, int debugMode)
{
    if(debugMode==1)
    {
        fprintf(stderr,"\nThe command: %s\n\n\n\n\n", pCmdLine->arguments[0]);
    }
    printf("executing\n");
    printf("arg[0]: %s\n",pCmdLine->arguments[0]);
    printf("arg[1]: %s\n",pCmdLine->arguments[1]);
    char * const arg1=pCmdLine->arguments[0];
    int ans=-1;

    pid_t pid = fork();

    if (pid == -1) {
        if(debugMode==1)
        {
            fprintf(stderr,"\nmy pid: %d\n\n\n\n\n", pid);
            perror("fork: ");
        }
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        if(debugMode==1)
        {
            fprintf(stderr,"\nmy pid: %d\n\n\n\n\n", pid);
        }
        printf("Hello from the child process!\n");

        ans=execvp(arg1,pCmdLine->arguments);
        sleep(2);      
        _exit(EXIT_SUCCESS);
    }
    else {
        if(debugMode==1)
        {
            fprintf(stderr,"\nmy pid: %d\n\n\n\n\n", pid);
        }
        /*This is the parent*/
        int status;
        printf("hello from dad\n");
        if(pCmdLine->blocking==1)
        {
            (void)waitpid(pid, &status, 0);
        }
        
        printf("Aba siem\n");
    }
    if(ans==-1)
    {
        perror("The error:");
    }
    else
    {
        printf("Aba siem\n");
    }
    
    freeCmdLines(pCmdLine);
    
}

void makeError()
{
    history
}