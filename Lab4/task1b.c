#include "lab4_util.h"


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

void Debugmode(int mode, int id, int ret);
void Printfiles(int mode, char* fileI, char* fileO);

int main (int argc , char* argv[], char* envp[]){
  
    int debug = 0;
    int input = 0;
    int output = 0;
    char *name1;
    char *name2;
    if(argc >1){
      int j;
      for(j=0; j<argc; j++){
        if(strncmp(argv[j],"-o",2)==0){
          name2 = argv[j]+2;
          output = 1;
        }
        if(strncmp(argv[j],"-i",2)==0){
          name1 = argv[j]+2;
          input = 1;
        }
        if(strncmp(argv[j],"-D",2)==0){
          debug = 1;
        }
      }
    }
    int op;
    int in;
    if(input == 0){
      name1 = "input";
    }
    if(output== 0){
      name2 = "stdout";
    }
    Printfiles(debug, name1, name2);
    /*
    in- output
    op- input
    */
    if(input==1){
      op = system_call(SYS_OPEN, name1, RDONLY, 0777);
      Debugmode(debug, SYS_OPEN, op);
      if(op <0){
        system_call(SYS_WRITE, STDERR, "problem in open the input file", 30);
        system_call(SYS_EXIT, 0, 0, 0);
        return 0;
      }
    }
    if(output==1){
      in = system_call(SYS_OPEN, name2, WRONLY, 0777);
      Debugmode(debug, SYS_OPEN, in);
      if(in <0){
        system_call(SYS_WRITE, STDERR, "problem in open the output file", 31);
        system_call(1, 0, 0, 0);
        return 0;
      }
    }
    int res;
    
    char *buf[1];
    if(input == 1){
      res = system_call(SYS_READ, op, buf, 1);
      Debugmode(debug, SYS_READ, res);
    }
    else{
      res = system_call(SYS_READ, STDIN, buf, 1);
      Debugmode(debug, SYS_READ, res);
     } 
      int index =1;
      while(1){
        char c = buf[0];
        if(c == -1){
          if(output==1){
            int cl = system_call(SYS_CLOSE, name1);
            Debugmode(debug, SYS_CLOSE, cl);
          }
          if(input==1){
            int cl = system_call(SYS_CLOSE, in);
            Debugmode(debug, SYS_CLOSE, cl);
          }    
          break;
        }
        if(c >= 'a' && c <='z'){
          c = c - ('a' - 'A');
        }
        buf[0] = c;
        if(output == 1){
          int ru = system_call(SYS_WRITE,in, buf,1);
          Debugmode(debug, SYS_WRITE, ru); 
          
        }
        else{
          int ru = system_call(SYS_WRITE,STDOUT, buf,1);
          Debugmode(debug, SYS_WRITE, ru);
        }

        if(input==1){
          res = system_call(SYS_READ, op, buf, 1);
          Debugmode(debug, SYS_READ, res);
        }
        else{
          res = system_call(SYS_READ, STDIN, buf, 1);
          Debugmode(debug, SYS_READ, res);
        }

        
        if(res ==0 ){
          if(output==1){
            int cl = system_call(SYS_CLOSE, name1);
            Debugmode(debug, SYS_CLOSE, cl);
          } 
          if(input==1){
            int cl = system_call(SYS_CLOSE, in);
            Debugmode(debug, SYS_CLOSE, cl);
          }         
          break;
        }
      }
      system_call(SYS_EXIT, 0, 0, 0);
      return 0;
  
    
}

void Debugmode(int mode, int id, int ret){
  if(mode==1){
    system_call(SYS_WRITE, STDERR, "\nthe id is: ", 12);
    system_call(SYS_WRITE, STDERR, itoa(id), strlen(itoa(id)));
    system_call(SYS_WRITE, STDERR, "\nthe value is: ", 15);
    system_call(SYS_WRITE, STDERR, itoa(ret), strlen(itoa(ret)));
    system_call(SYS_WRITE, STDERR, "\n", 1);
  }
}

void Printfiles(int mode, char* fileI, char* fileO){
  if(mode==1){
    system_call(SYS_WRITE, STDERR, "\ninput file: ", 12);
    system_call(SYS_WRITE, STDERR, fileI, strlen(fileI));
    system_call(SYS_WRITE, STDERR, "\noutput file: ", 13);
    system_call(SYS_WRITE, STDERR, fileO, strlen(fileO));
    system_call(SYS_WRITE, STDERR, "\n", 1);
  }
}

  



