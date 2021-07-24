#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE * output=stdout;
    FILE * input=stdin;
    char modifiedChar;
    //bool debugMode=false;
    char debugMode=0;
    int encriptionArgc=0;
    int encriptionMode=0;
    int encriptionStringCounter=0;
    int differentInput=0;
    int differentOutput=0;
    char *inputFileName,*outputFileName;

    char delta=0;
    char c=' ';
    for (size_t i = 1; i < argc; i++)
    {
        if(strncmp(argv[i],"-D",2)==0){
            debugMode=1;
        }
        if(strncmp(argv[i],"-e",2)==0 || strncmp(argv[i],"+e",2)==0){
            if(argv[i][2]!='\0')
            {
                
                encriptionMode=1;
                encriptionArgc=i;
            }
        }
        if(strncmp(argv[i],"-o",2)==0)
        {
            //Output
            differentOutput=1;
            outputFileName=argv[i];
            outputFileName+=2;
            output=fopen(outputFileName,"w");
        }
        if(strncmp(argv[i],"-i",2)==0)
        {
            differentInput=1;
            inputFileName=argv[i];
            inputFileName+=2;
            input=fopen(inputFileName,"r");
        }

    }

    
    //check for debug mode
    
    while (c!=EOF)
    {
        c=fgetc(input);    
        while (c!=EOF && c!='\n')
        {
                modifiedChar=c;
                if(encriptionMode==0 && c>='a' && c<='z')
                {
                    modifiedChar=c-('a'-'A');
                }
                //Qestion if needed caps anyway
                if(encriptionMode==1)
                {
                    
                    if(argv[encriptionArgc][2+encriptionStringCounter]=='\0')
                    {
                        encriptionStringCounter=0;
                    }
                    delta=(argv[encriptionArgc][2+encriptionStringCounter])-'0';
                    if(argv[encriptionArgc][0]=='+')
                    {
                        modifiedChar=c+delta;
                    }
                    else
                    {
                        modifiedChar=c-delta;
                    }
                    
                    
                    encriptionStringCounter++;
                }

                if(debugMode==1)
                {
                        fprintf(stderr,"%d\t%d\n",c,modifiedChar);   
                }
                fputc(modifiedChar,output);
                c=fgetc(input);
        }
        fprintf(output,"\n");
    }
    if(differentInput==1)
    {
        fclose(input);
    }
    if(differentOutput)
    {
        fclose(output);
    }
    return 0;
}
