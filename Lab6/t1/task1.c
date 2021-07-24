#include <linux/limits.h>

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include "LineParser.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>



#define WCONTINUED 8
#define MAX_LINE 2048
void makeError();


typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;


typedef struct{
    char *name;
    char *value;
    struct link *nextNode;
} mylink;


mylink* linkedList;

/*process **process_list=NULL;*/

void execute(process **process_list,cmdLine *pCmdLine, int dMode);

void proccessStatusLabel(char* buf, int status);
void theRest(process* newProcess, cmdLine* cmd, pid_t pid);
void updateProcessStatus(process* process_list, int pid, int status);
void updateProcessList(process **process_list);


#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0


void addProcess(process **process_list, cmdLine* cmd, pid_t pid, int status)
{
    if(pid>0)
    {
        process *newProcess= calloc(1,sizeof(process));
        newProcess->cmd=cmd;
        newProcess->pid=pid;
        if(status==1)
        {
            newProcess->status=TERMINATED;/*TODO: ask the TA?*/ 
        }
        else
        {
            newProcess->status=RUNNING;
            printf("\nmake me running\n");
        }
        
        newProcess->next=NULL;
        process *head=*process_list;

        if(head!=NULL)
        {
            process *currentProcess= head;
            
            while(currentProcess->next!=NULL)
            {
                currentProcess=currentProcess->next;
            }
            currentProcess->next=newProcess;
        }
        else
        {
            *process_list=newProcess;
        }
     }
}

void freeProcessList(process* process_list)
{
    if(process_list!=NULL)
    {
        process *currentProcess= process_list;
        process *nextProcess;
        while(currentProcess!=NULL)
        {
            freeCmdLines(currentProcess->cmd);
            nextProcess=currentProcess->next;
            free(currentProcess);
            currentProcess=nextProcess;
        }
    }
}

void updateProcessList(process **process_list)
{
    process *head=*process_list;
    if(head!=NULL)
    {
        process *currentProcess= head;
        while(currentProcess!=NULL)
        {
            int status;
            int ans=waitpid(currentProcess->pid,&status,WNOHANG|WCONTINUED);

            if(ans==-1)
            {
                perror("waitpid: ");
                exit(EXIT_FAILURE);
            }
            if(WIFCONTINUED(status))
            {
                if(currentProcess->status!=RUNNING)
                {
                    printf("\nchanging to RUNNING!!!\n");
                    
                    updateProcessStatus(currentProcess,currentProcess->pid,RUNNING);
                }
            }
            else if(WIFEXITED(status))
            {
                if(currentProcess->status!=TERMINATED)
                {
                    printf("\nchanging to TERMINATED!!!\n");
                    printf("new status: %d\n",status);
                    printf("pid: %d\n",currentProcess->pid);
                    printf("current status: %d\n",currentProcess->status);
                    updateProcessStatus(currentProcess,currentProcess->pid,TERMINATED);
                }
            }
            else if(WIFSTOPPED(status))
            {
                if(currentProcess->status!=SUSPENDED)
                {
                    printf("\nchanging to SUSPENDED!!!\n");
                    updateProcessStatus(currentProcess,currentProcess->pid,SUSPENDED);
                }
            }
            currentProcess=currentProcess->next;
        }
    }
}

void updateProcessStatus(process* process_list, int pid, int status)
{
    process_list->status=status;
}

process* lookForProcessInList(process** process_list, int pid)
{
    process *head=*process_list;
    process *currentProcess= NULL;
    int found=0;
    if(head!=NULL)
    {
        currentProcess= head;
        while(currentProcess!=NULL && found==0)
        {
            if(currentProcess->pid==pid)
            {
                found=1;
            }
            else
            {
               currentProcess=currentProcess->next;
            }
        }
    }
    if(found==1)
    {
        return currentProcess;
    }
    else
    {
        return NULL;
    }
    
            
            
}


void printProcessList(process **process_list)
{
    updateProcessList(process_list);
    printf("INDEX\t\tPID\t\tStatus\t\tCommand\n");
    process* head=*process_list;
    process *currentProcess= head;
    process *prevProcess=head;
    int shouldDeleteCurrent=0;
    int i=0;
    while(currentProcess!=NULL)
    {
        
        if(currentProcess->status==TERMINATED)
        {
            printf("%d\t\t%d\t\t%s\t\t",i,currentProcess->pid,"TERMINATED");
            shouldDeleteCurrent=1;            
        }
        else if(currentProcess->status==RUNNING)
        {
            printf("%d\t\t%d\t\t%s\t\t",i,currentProcess->pid,"RUNNING");
        }
        else if(currentProcess->status==SUSPENDED)
        {
            printf("%d\t\t%d\t\t%s\t\t",i,currentProcess->pid,"SUSPENDED");
        }
        
        int j;
        for ( j = 0; j < currentProcess->cmd->argCount; j++)
        {
            printf("%s ",currentProcess->cmd->arguments[j]);
        }
        
        
        printf("\n");
        if(shouldDeleteCurrent==1)
        {
            process* nextProcess=currentProcess->next;
            if(prevProcess==currentProcess && nextProcess!=NULL)
            {
                freeCmdLines(currentProcess->cmd);
                free(currentProcess);
                head=nextProcess;
                *process_list=nextProcess;
                currentProcess=nextProcess;
                prevProcess=nextProcess;
            }
            else if(prevProcess==currentProcess && nextProcess==NULL)
            {
                freeCmdLines(currentProcess->cmd);
                free(currentProcess);
                process_list=NULL;
                currentProcess=NULL;
                prevProcess=NULL;
            }
            else if(nextProcess!=NULL)
            {
                prevProcess->next=nextProcess;
                freeCmdLines(currentProcess->cmd);
                free(currentProcess);
                currentProcess=prevProcess;
            }
            else
            {
                
                freeCmdLines(currentProcess->cmd);
                free(currentProcess);
                prevProcess->next=NULL;
                currentProcess=NULL;
            }
        }

        if(i>0)
        {
            prevProcess=currentProcess;
        }
        if(shouldDeleteCurrent==0)
        {
            currentProcess=currentProcess->next;
        }
        i++;
        shouldDeleteCurrent=0;
    }
    printf("\n");
    
}


void proccessStatusLabel(char* buf, int status)
{

    if(status==TERMINATED)
    {
        buf="TERMINATED";
    }
    else if(status==RUNNING)
    {
        buf="RUNNING";
    }
    else if(status==SUSPENDED)
    {
        buf="SUSPENDED";
    }
}


int searchAndInsertNewVariableNode(char *name, char *value)
{
    int answer=1;
    mylink *currentLink=linkedList;
    
    while(answer>0 && currentLink!=NULL)
    {
        if(strcmp(currentLink->name,name)==0)
        {
            answer=-1;
        }
        else
        {
            if(currentLink->nextNode==NULL)
            {
                break;
            }
            else
            {
                currentLink=currentLink->nextNode;
            }
        }
    }

    if(answer>=0)
    {
        mylink *newLink= calloc(1,sizeof(link));
        newLink->name=calloc(strlen(name),sizeof(char));
        newLink->value=calloc(strlen(value),sizeof(char));
        int i;
        for(i=0;i<strlen(name);i++)
        {
            newLink->name[i]=name[i];
        }
        int j;
        for(j=0;j<strlen(value);j++)
        {
            newLink->value[j]=value[j];
        }
        newLink->nextNode=NULL;
        if(currentLink==NULL)
        {
            linkedList=newLink;
        }
        else
        {
            currentLink->nextNode=newLink;
        }
        
        
    }
    else
    {
        
        free(currentLink->value);
        int i;
        for(i=0;i<strlen(value);i++)
        {
            currentLink->value[i]=value[i];
        }

    }
    
    return answer;
}

void printVarsList()
{
    mylink *currentLink=linkedList;
    printf("the saved variables: \n");
    while(currentLink!=NULL)
    {
        printf("var=%s , value=%s\n",currentLink->name,currentLink->value);
        currentLink=currentLink->nextNode;
    }
}

char* getValueByName(char* name)
{
    int found=0;
    char *value=NULL;
    mylink *currentLink=linkedList;
    while(found==0 && currentLink!=NULL)
    {
        if(strcmp(name,currentLink->name)==0)
        {
            value=currentLink->value;
            found=1;
        }
        currentLink=currentLink->nextNode;
    }

    return value;
}

int main(int argc, char **argv) {
    
    printf("home: %s\n",getenv("HOME"));
    int debugMode=0;
    int j;
    int pipefd[2];
    process * ptr;
    process ** process_list=calloc(1,sizeof(ptr));
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
        else if(strcmp(lineFromTheUser,"procs\n")==0)
        {
            printProcessList(process_list);
        }
        
        else{
            nextCommand=parseCmdLines(lineFromTheUser);
            
            if(strcmp(nextCommand->arguments[0],"cd")==0)
            {
                    int ans=chdir(nextCommand->arguments[1]);
                    if(ans==-1)
                    {
                        perror("chdir: ");
                    }
                
                
            }
            else if(strcmp(nextCommand->arguments[0],"suspend")==0)
            {
                int pid=atoi(nextCommand->arguments[1]);
                int ans=kill(pid,SIGTSTP);
                if(ans==0)
                {
                    printf("suspending: %d",ans);
                    /*updateProcessList(process_list);*/
                    process *toChange=lookForProcessInList(process_list,pid);
                    if(toChange!=NULL)
                    {
                        toChange->status=SUSPENDED;
                    }
                    else
                    {
                        perror("process not found:");
                    }
                    
                }
                else
                {
                    perror("kill-suspend:");
                }
                

            }
            else if(strcmp(nextCommand->arguments[0],"kill")==0)
            {
                int pid=atoi(nextCommand->arguments[1]);
                int ans=kill(pid,SIGINT);

                if(ans==0)
                {
                    printf("killing: %d",ans);
                    updateProcessList(process_list);
                    process *toChange=lookForProcessInList(process_list,pid);
                    if(toChange!=NULL)
                    {
                        toChange->status=TERMINATED;
                    }
                    else
                    {
                        perror("process not found:");
                    }
                }
                else
                {
                    perror("kill-killing:");
                }

                
            }
            else if(strcmp(nextCommand->arguments[0],"wake")==0)
            {
                int pid=atoi(nextCommand->arguments[1]);
                int ans=kill(pid,SIGCONT);

                if(ans==0)
                {
                    printf("waking: %d",ans);
                    updateProcessList(process_list);
                    process *toChange=lookForProcessInList(process_list,pid);
                    if(toChange!=NULL)
                    {
                        toChange->status=RUNNING;
                    }
                    else
                    {
                        perror("process not found:");
                    }
                }
                else
                {
                    perror("kill-waking:");
                }

                
            }
            else{
                execute(process_list,nextCommand, debugMode);
                }
        }
        
    }
}

void execute(process** process_list,cmdLine *pCmdLine, int debugMode)
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
        if(pCmdLine->inputRedirect!=NULL)
        {
            fclose(stdin);
            fopen(pCmdLine->inputRedirect,"r");
        }
        if(pCmdLine->outputRedirect!=NULL)
        {
            fclose(stdout);
            fopen(pCmdLine->outputRedirect,"w");
        }
        
        ans=execvp(arg1,pCmdLine->arguments);
        sleep(2);      
        if(pCmdLine->inputRedirect!=NULL)
        {
            fclose(pCmdLine->inputRedirect);
            fopen(stdin,"r");
        }
        if(pCmdLine->outputRedirect!=NULL)
        {
            fclose(pCmdLine->outputRedirect);
            fopen(stdout,"w");
        }
        
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
            
            addProcess(process_list,pCmdLine,pid, 1);
        }
        else
        {
            addProcess(process_list,pCmdLine,pid, 0);
        }
        
        printf("dad finished\n");
    }
    if(ans==-1)
    {
        perror("The error:");
    }
    else
    {
        printf("Aba siem\n");
    }
    
    /*freeCmdLines(pCmdLine);*/
    
}

void makeError()
{
    
}