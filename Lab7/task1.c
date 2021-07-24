#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>

int debugMode=0;
int displayMode=0;

struct fun_desc {
  char *name;
  char (*fun)(char);
};

typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;
char* unit_to_format(int unit, state* s) ;


void print_units(FILE* output, char* buffer, int count, state* s) {
    int unit_size = s->unit_size;
    char* end = buffer + unit_size*count;
    while (buffer < end) {
        //print ints
        int var = *((int*)(buffer));
        fprintf(output, unit_to_format(unit_size, s), var);
        buffer += unit_size;
    }
}


char* unit_to_format(int unit, state* s) {
    static char* formats[] = {"%#hhx\n", "%#hx\n", "No such unit", "%#x\n"};
    int unit_size = s->unit_size;
    return formats[unit_size-1];
    /* If the above is too confusing, this can also work:
    switch (unit_size) {
        case 1:
            return "%#hhx\n";
        case 2:
            return "%#hx\n";
        case 4:
            return "%#hhx\n";
        default:
            return "Unknown unit";
    }
    */
}  



void ToggleDebugMode (state *s)
{
    if(debugMode==1)
    {
        debugMode=0;
        printf("Debug mode now off\n");
    }
    else
    {
        debugMode=1;
        printf("Debug mode now on\n");
    }
}

void Quit (state *s)
{
    if(debugMode==1)
    {
        printf("quiting\n");
    }

    exit(0);
}

void SetFileName (state *s)
{
    for (int h=0; h<128; h++){
        s->file_name[h] ='\0'; 
    }
    printf("Enter file name:\n") ;
    char buf[100];
    fgets(buf,100,stdin);
    for (size_t i = 0; i < strlen(buf); i++)
    {
        if(buf[i]!='\n')
        {
            s->file_name[i]=buf[i];
        }
    }
    if(debugMode==1)
    {
        printf(buf);
        printf("\n");
    }
    
}

void SetUnitSize (state *s)
{
    int size;
    printf("Enter unit size:\n") ;
    char buf[100];
    fgets(buf,100,stdin);
    sscanf(buf,"%d",&size);
    if(size==1 || size== 2 || size==4)
    {
        s->unit_size=size;
        if(debugMode==1)
        {
            printf("Debug: set size to %d",size);
            printf("\n");
        }
    }
    else
    {
        printf("size not valid\n");
    }
    
}

void LoadIntoMemory (state *s)
{
    if(strcmp(s->file_name,"")==0)
    {
        printf("Error: file name empty\n");
        return;
    }
    FILE* fd=fopen(s->file_name,"rb");
    if(fd==NULL)
    {
        printf("Error: fail to open file\n");
        return;
    }

    /*loaction*/
    int loaction;
    printf("Enter location:\n") ;
    char locationBuf[10];
    fgets(locationBuf,10,stdin);
    sscanf(locationBuf,"%X",&loaction);

    /*length*/
    int length;
    printf("Enter length:\n") ;
    char lengthBuf[10];
    fgets(lengthBuf,10,stdin);
    sscanf(lengthBuf,"%X",&length);

    if(debugMode==1)
    {
        printf("File name: %s\n",s->file_name);
        printf("File location: %d\n",loaction);
        printf("File length: %d\n",length);
    }

    int res=fseek(fd,loaction,SEEK_SET);
    if(res!=0)
    {
        printf("Error: fseek file\n");
        return;
    }
    int theNumberOfBytesToRead=length*s->unit_size;
    int loaded=fread(s->mem_buf,1,theNumberOfBytesToRead,fd);
    printf("Loaded %X units into memory\n",loaded);
    fclose(fd);
    
}

void ToggleDisplayMode (state *s)
{
    if(displayMode==1)
    {
        displayMode=0;
        printf("Display flag now off, decimal represntation\n");
    }
    else
    {
        displayMode=1;
        printf("Display flag now on, hexadecimal represntation\n");
    }

    
    
}

void MemoryDisplay (state *s)
{
    
     /*address*/
    int address;
    printf("Enter address:\n") ;
    char addressBuf[15];
    fgets(addressBuf,15,stdin);
    sscanf(addressBuf,"%X",&address);

    /*length*/
    int length;
    printf("Enter length:\n") ;
    char lengthBuf[15];
    fgets(lengthBuf,15,stdin);
    sscanf(lengthBuf,"%d",&length);
    int unit=s->unit_size;
    int size=unit*length;
    char buf[size];
    
    if(address==0)
    {
        if(displayMode==1)
        {
            printf("Hexadecimal\n");
            printf("===========\n");
            print_units(stdout, s->mem_buf, length, s);
        }
        else
        {
            printf("Decimal\n");
            printf("===========\n");
        
        
        
        for (size_t i = 0; i < length && i<10000; i=i+unit)
        {
            
           

            
            unsigned int numberToPrint =0; 

                    /*decimal*/
                    for (size_t j = 0; j < unit; j++)
                {
                    numberToPrint = numberToPrint + s->mem_buf[j+i];
                    
                }
                printf("%ud\n", numberToPrint);
    
            }
        }
    }
    else
    {
        FILE *fd=fopen(s->file_name,"rb");
        if(fd==NULL)
        {
            printf("error opening file\n");
            return;
        }
        int res=fseek(fd,address,SEEK_SET);
        if(res==-1)
        {
            printf("error fseek\n");
            return;
        }
        res=fread(buf,s->unit_size,length,fd);
        if(res==-1)
        {
            printf("error fread\n");
            return;
        }
        if(displayMode==1)
        {
            printf("Hexadecimal\n");
            printf("===========\n");
            print_units(stdout, buf, length, s);

        }
        else
        {
            printf("Decimal\n");
            printf("===========\n");

        

        for (size_t i = 0; i < length && i<10000; i=i+unit)
        {
            
            

                    unsigned int numberToPrint =0; 

                    /*decimal*/
                    for (size_t j = 0; j < unit; j++)
                {
                    numberToPrint = numberToPrint + buf[j+i];
                    
                }
                printf("%ud\n", numberToPrint);

            
            
        }
        }
        fclose(fd);
    }  

}

void SaveIntoFile (state *s)
{
            printf("%s\n", s->file_name);
    FILE *fdWrite=fopen(s->file_name,"r+");
     /*sourceAddress*/
    int sourceAddress;
    printf("Enter source address:\n") ;
    char sourceAddressBuf[15];
    fgets(sourceAddressBuf,15,stdin);
    sscanf(sourceAddressBuf,"%X",&sourceAddress);

    /*targetLocation*/
    int targetLocation;
    printf("Enter source target location:\n") ;
    char targetLocationBuf[15];
    fgets(targetLocationBuf,15,stdin);
    sscanf(targetLocationBuf,"%X",&targetLocation);

    /*length*/
    int length;
    printf("Enter length:\n") ;
    char lengthBuf[15];
    fgets(lengthBuf,15,stdin);
    sscanf(lengthBuf,"%d",&length);
    int unit=s->unit_size;
    int size=unit*length;
    char buf[size];
    
    if(sourceAddress==0)
    {
        printf("targetLocation:%X\n", targetLocation);
        printf("file name:%s\n", s->file_name);
        printf("fdWrite:%x\n", fdWrite);
        

        int ans=fseek(fdWrite,targetLocation,SEEK_SET);
        printf("AB@\n");

        if(ans==-1)
        {
            printf("error fseek\n");
            return;
        }
        printf("ABC@\n");

        char *wrBuf=s->mem_buf;
        if(debugMode==1)
        {
        printf("unit:%d\n",unit);
        printf("*wrBuf:%X\n",wrBuf);
        printf("length:%d\n",length);
        printf("fdWrite:%d\n",fdWrite);
        }
        fwrite(wrBuf,unit,length,fdWrite);
        if(debugMode==1)
        {
            printf("the first read byte:%x\n",s->mem_buf[0]);
        }
    }
    else
    {
        int res=fseek(fdWrite,sourceAddress,SEEK_SET);
        if(res==-1)
        {
            printf("error fseek\n");
            return;
        }
        res=fread(buf,s->unit_size,length,fdWrite);
        if(res==-1)
        {
            printf("error fread\n");
            return;
        }
        char *wrBuf=buf[sourceAddress];
        fwrite(wrBuf,unit,length,fdWrite);
        if(debugMode==1)
        {
            printf("the first read byte:%x\n",buf[0]);
        }
        
    }  
    fclose(fdWrite);
}

void MemoryModify (state *s)
{
    FILE *fdModify=fopen(s->file_name,"r+");
     /*location*/
    int location;
    printf("Enter location:\n") ;
    char locationBuf[15];
    fgets(locationBuf,15,stdin);
    sscanf(locationBuf,"%X",&location);

    /*value*/
    int value;
    printf("Enter value:\n") ;
    char valueBuf[15];
    fgets(valueBuf,15,stdin);
    sscanf(valueBuf,"%X",&value);
    if(debugMode==1)
    {
        printf("Location: %X\n",location);
        printf("Value: %X\n",value);
    }
    int unit=s->unit_size;
    int res=fseek(fdModify,location,SEEK_SET);
    if(res==-1)
    {
        printf("fseek error\n");
        return;
    }
    /*res=fwrite(valueBuf,unit,1,fdModify);*/
    res=fwrite(&value,unit,1,fdModify);
    fclose(fdModify);
    
}

void map(state *s, int array_length, char (*f) (state*));

void map(state *s, int array_length, char (*f) (state*)){
        f(s);
}


int main(int argc, char **argv){
  state *idoState=calloc(1,sizeof(state));


  struct fun_desc menu[] = {    { "Toggle Debug Mode", ToggleDebugMode },
                                { "Set File Name", SetFileName },
                                { "Set Unit Size", SetUnitSize },
                                { "Load Into Memroy", LoadIntoMemory },
                                { "Toggle Display Mode", ToggleDisplayMode },
                                { "Memory Display", MemoryDisplay },
                                { "Save Into File", SaveIntoFile },
                                { "Memory Modify", MemoryModify },
                                { "Quit", Quit},
                                { NULL, NULL }};

  int sizeOfmenuArray=0;
  while (menu[sizeOfmenuArray].fun!=NULL || menu[sizeOfmenuArray].name!=NULL )
  {
    sizeOfmenuArray++;
  }
  printf("Please choose a function:\n");
  for (size_t i = 0; i < sizeOfmenuArray; i++)
  {
    printf("%d-%s\n",i,menu[i].name);
  }
  int choose;
  char buff[10];
  fgets(buff,5,stdin);  
  choose=atoi(buff);
  while (choose>=0 && choose<=sizeOfmenuArray)
  {
    map(idoState,5,menu[choose].fun);
    
    printf("Please choose a function:\n");
    for (size_t i = 0; i < sizeOfmenuArray; i++)
    {
      printf("%d) %s\n",i,menu[i].name);
    }
    char buff2[10];
    fgets(buff2,5,stdin);
    choose=atoi(buff2);
  }
  
  exit(0);
}