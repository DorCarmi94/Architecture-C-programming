#include <linux/limits.h>

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define MAX_ARGUMENTS 256
#define WCONTINUED 8
#define MAX_LINE 2048
int main(int argc, char **argv) {
    int dMode=0;
    int i;
    for (i=1; i < argc; i++)
    {
        if(strncmp(argv[i],"-d",2)==0)
        {
            dMode=1;
        }
    }


    int pipefdChild[2];
    pid_t child1;
    pid_t child2;
    char bufChild1;
    char budChild2;

    if (pipe(pipefdChild) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if(dMode)
    {
        printf("(parent_process>forking…)\n");
    }
    child1 = fork();
    if (child1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0) {    /* Child reads from pipe */
        //pipefd[0]-read
        //pipefd[1]-write
        int cstdou= close(STDOUT_FILENO);
        int dupFd=dup(pipefdChild[1]);
        printf("dupFD1: %d\n",dupFd);
        close(pipefdChild[1]);
        if(dMode)
        {
            fprintf(stderr,"(child1>redirecting stdout to the write end of the pipe…)\n");
        }
        char *arguments[MAX_ARGUMENTS];
        
        arguments[0]="ls";
        arguments[1]="-l";
        arguments[2]='\0';
        if(dMode)
        {
            fprintf(stderr,"(child1>going to execute cmd: …)\n");
        }
        
        int i=execvp(arguments[0],arguments);
        if(dMode)
        {
            printf("(child1>exiting....\n");
        }
        
    }
    else {            /* Parent */
        if(dMode)
        {
            fprintf(stderr,"(parent_process>created process with id: %d )\n",child1);
            fprintf(stderr,"(parent_process>closing the write end of the pipe…)\n");
        }
        
        close(pipefdChild[1]);
        if(dMode)
        {
            fprintf(stderr,"(parent_process>waiting for child processes to terminate…)\n");
        }
        int status1;
        (void)waitpid(child1, &status1, 0);
        child2=fork();
                    if (child2 == -1) {
                        perror("fork");
                        exit(EXIT_FAILURE);
                    }

                    if (child2 == 0) {    /* Child2 reads from pipe */
                        //pipefd[0]-read
                        //pipefd[1]-write
                        close(STDIN_FILENO);
                        int dupFd2=dup(pipefdChild[0]);
                        printf("dupFD2: %d\n",dupFd2);
                        close(pipefdChild[0]);
                        if(dMode)
                        {
                            printf("(child2>redirecting stdout to the write end of the pipe…)\n");
                        }
                        char *arguments2[MAX_ARGUMENTS];
                        
                        arguments2[0]="tail";
                        arguments2[1]="-n";
                        arguments2[2]="2";
                        arguments2[3]='\0';
                        if(dMode)
                        {
                            printf("(child2>going to execute cmd: …)\n");
                        }
                        int i_2=execvp(arguments2[0],arguments2);
                        
                    }
                    else {            /* Parent */
                        if(dMode)
                        {
                            printf("(parent_process>created process with id: %d )\n",child2);
                            printf("(parent_process>closing the read end of the pipe…)\n");
                        }
                        close(pipefdChild[0]);          /* Close unused read end */
                        
                        int status2;
                        
                        
                        if(dMode)
                        {
                            printf("(parent_process>waiting for child processes to terminate…)\n");
                        }
                        (void)waitpid(child2, &status2, 0);  
                        if(dMode)
                        {
                            printf("(parent_process>exiting…)\n");
                        }
                              
                    }     
        }
}


