#include "util.h"

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


int main(int argc, char* argv[], char* envp[]) {
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
           print_type(buf,index,entp);
        }
        else if(pMode==0)
        {
           printFileNames(debug,entp->buf,entp->len);
           print_type(buf,index,entp);
        }
        entp=(struct ent*)(buf+index);
        index=index+entp->len;
    }

    return 0;
}

void print_type(char* buf, int index, ent* entp){
    int type = *(buf+index+entp->len-1);
    system_call(4,2,"\ntype: ", 7);
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
        case 12:system_call(4,2,"unix\n", 5);
            break;    
        case 0:system_call(4,2,"zero\n", 5);
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