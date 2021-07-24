#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 100
typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;
typedef struct link link;
struct link {
    link *nextVirus;
    virus *vir;
};


virus* readVirus(FILE *filePtr)
{
    virus *newVirus;

    unsigned short sizeOfSignature;

    if(fread(&sizeOfSignature,1,2,filePtr)>0)
    {
        newVirus=calloc(1,sizeof(virus));
        newVirus->SigSize=sizeOfSignature;

        newVirus->sig=calloc(sizeOfSignature,sizeof(char));   

        fread(newVirus->virusName,1,16,filePtr);
        fread(newVirus->sig,1,sizeOfSignature,filePtr);
        
        return newVirus; 
    }
    else
    {
        return NULL;
    }
    
}

void printVirus(virus* virus, FILE* output)
{
    fprintf(output,"Virus name: %s\n",virus->virusName);
    fprintf(output,"Virus size: %d\n",virus->SigSize);
    fprintf(output,"signature:\n");
    unsigned char *sigPtr=virus->sig;
    for (size_t i = 0; i < virus->SigSize; i++)
    {
        fprintf(output,"%02X ",sigPtr[i]);
    }
    fprintf(output,"\n");
}


 /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
void list_print(link *virus_list, FILE*fileOutput)
{
    if(virus_list!=NULL)
    {
        if(virus_list->vir!=NULL)
        {
            printVirus(virus_list->vir,fileOutput);
            list_print(virus_list->nextVirus,fileOutput);
        }
    }
}

/* Add a new link with the given data to the list 
        (either at the end or the beginning, depending on what your TA tells you),
        and return a pointer to the list (i.e., the first link in the list).
        If the list is null - create a new entry and return a pointer to the entry. */ 
link* list_append(link* virus_list, virus* data)
{
    link *newLink=calloc(1,sizeof(link));
    newLink->vir=data;
    if(virus_list!=NULL)
    {
        newLink->nextVirus=virus_list;
    }
    return newLink;
}
     
/* Free the memory allocated by the list. */
void list_free(link *virus_list)
{
    if(virus_list!=NULL)
    {
        if(virus_list->nextVirus!=NULL)
        {
            
            list_free(virus_list->nextVirus);
            free(virus_list->nextVirus);            
        }
        free(virus_list->vir->sig);
        free(virus_list->vir);
        
    }
}

void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
    
    for (size_t i = 0; i < size; i++)
    {
        link *nextVirusLink=virus_list;
        while (nextVirusLink!=NULL)
        {
            unsigned char* currentVirusSignature=nextVirusLink->vir->sig;
            char* currentStringFromFileBuffer=buffer+i;
            unsigned short sizeOfVirus=nextVirusLink->vir->SigSize;
            int cmpRezult=memcmp(currentVirusSignature,currentStringFromFileBuffer,sizeOfVirus);
            if(cmpRezult==0)
            {
                printf("Starting byte location of the virus: %d\n",i);
                printVirus(nextVirusLink->vir,stdout);
            }
            nextVirusLink=nextVirusLink->nextVirus;
        }
        
    }
    
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize)
{
    char writeBuffer[signitureSize];
    for (size_t i = 0; i < signitureSize; i++)
    {
        writeBuffer[i]=0x90;
    }
    
    FILE *file=fopen(fileName,"r+");

    fseek(file,signitureOffset,SEEK_SET);
    fwrite(writeBuffer,sizeof(char),signitureSize,file);
    fclose(file);
    printf("Fixed!!\n");
}

int main(int argc, char **argv) {
    link *virusList=NULL;
    while(1)
    {
        printf("1) Load signatures\n");
        printf("2) Print signatures\n");
        printf("3) Detect viruses\n");
        printf("4) Fix file\n");
        printf("5) Quit\n");
        char readChoice[BUFFER_SIZE];
        fgets(readChoice,BUFFER_SIZE, stdin);
        int choiceNumber=atoi(readChoice);
        
        if(choiceNumber==1)
        {
            char fileName[BUFFER_SIZE];
            fgets(fileName,BUFFER_SIZE,stdin);
            int length=strlen(fileName);
            char fileConvName[length];
            char *strPtr=fileConvName;
            sscanf(fileName,"%s",strPtr);
            FILE *sigFile=fopen(fileConvName,"rb");
            virus *newVirus=readVirus(sigFile);
            virusList=list_append(NULL,newVirus);
            while ((newVirus=readVirus(sigFile))!=NULL)
            {
                virusList=list_append(virusList,newVirus);//1B
            }
            fclose(sigFile);
            printf("\n");
        }
        if(choiceNumber==2)
        {
            printf("2");    
            if(virusList!=NULL)
            {
                list_print(virusList,stdout);
            }
        }
        if(choiceNumber==3)
        {
            char fileName[BUFFER_SIZE];
            fgets(fileName,BUFFER_SIZE,stdin);
            int length=strlen(fileName);
            char fileConvName[length];
            char *strPtr=fileConvName;
            sscanf(fileName,"%s",strPtr);
            FILE *myFile=fopen(fileConvName,"rb"); //-----------Open--------------
            char *buffer=calloc(10000,sizeof(char));
            fseek (myFile, 0, SEEK_END);
            unsigned int sizeOfFile=ftell (myFile);
            printf("Size of file: %d\n",sizeOfFile);
            fseek(myFile,0,SEEK_SET);
            fread(buffer,1,sizeOfFile,myFile);
            detect_virus(buffer,sizeOfFile,virusList);
            fclose(myFile); //------ Close-------------
            free(buffer);
            printf("\n");
        }
        if(choiceNumber==4)
        {
            char fileName[BUFFER_SIZE];
            char location[BUFFER_SIZE];
            char size[BUFFER_SIZE];

            fgets(fileName,BUFFER_SIZE,stdin);
            int length=strlen(fileName);
            char fileConvName[length];
            char *strPtr=fileConvName;
            sscanf(fileName,"%s",strPtr);


            fgets(location,BUFFER_SIZE,stdin);
            int theLocation;
            sscanf(location,"%d",&theLocation);
            


            fgets(size,BUFFER_SIZE,stdin);
            int theSize;
            sscanf(size,"%d",&theSize);
            

            kill_virus(fileConvName,theLocation,theSize);
        }
        if(choiceNumber==5)
        {
            
            list_free(virusList);
            free(virusList);
            exit(0);
        }
    }
    
}