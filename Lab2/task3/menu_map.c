#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fun_desc {
  char *name;
  char (*fun)(char);
};

char encrypt(char c); /* Gets a char c and returns its encrypted form by adding 3 to its value. 
          If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c); /* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */
char dprt(char c); /* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
char cprt(char c); /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
char my_get(char c); /* Ignores c, reads and returns a character from stdin using fgetc. */

char* map(char *array, int array_length, char (*f) (char));

char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
} 
char* map(char *array, int array_length, char (*f) (char)){
    char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
    for (size_t i = 0; i < array_length; i++)
    {
        mapped_array[i]=f(array[i]);
    }
    free(array);
    return mapped_array;
}


/* Gets a char c and returns its encrypted form by adding 3 to its value. 
          If c is not between 0x20 and 0x7E it is returned unchanged */
char encrypt(char c)
{
  if(c>=0x20 && c<=0x7E)
  {
    return c+3;
  }
  else
  {
    return c;
  }
  
}

/* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c)
{
  if(c>=0x20 && c<=0x7E)
  {
    return c-3;
  }
  else
  {
    return c;
  }
}

/* dprt prints the value of c in a decimal representation followed by a 
           new line, and returns c unchanged. */
char dprt(char c)
{
  printf("%d\n",c);
  return c;
}

/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
char cprt(char c)
{
  if(c>=0x20 && c<=0x7E)
  {
    printf("%c\n",c);
  }
  else
  {
    printf(".\n");
  }
  return c;
} 
 /* Ignores c, reads and returns a character from stdin using fgetc. */
char my_get(char c)
{
  char ch=fgetc(stdin);
  return ch;
}

 /* Gets a char c,  and if the char is 'q' , 
 ends the program with exit code 0. 
 Otherwise returns c. */
char quit(char c)
{
  if(c=='q')
  {
    exit(0);
  }
  else
  {
    return c;
  }
}

/*
0) Censor
1) Encrypt
2) Decrypt
3) Print dec
4) Print string
5) Get string
6) Quit
*/
 
int main(int argc, char **argv){
  char *carrayPtr=malloc(sizeof(char)*5);

  struct fun_desc menu[] = {  { "Censor", censor },   { "Encrypt", encrypt }, 
                              { "Decrypt", decrypt },   { "Print dec", dprt },
                              { "Print string", cprt },   { "Get string", my_get },
                              { "Quit", quit },  { NULL, NULL }};

  int sizeOfmenuArray=0;
  while (menu[sizeOfmenuArray].fun!=NULL || menu[sizeOfmenuArray].name!=NULL )
  {
    sizeOfmenuArray++;
  }
  printf("Please choose a function:\n");
  for (size_t i = 0; i < sizeOfmenuArray; i++)
  {
    printf("%d) %s\n",i,menu[i].name);
  }
  int choose;
  char buff[10];
  fgets(buff,5,stdin);  
  choose=atoi(buff);
  while (choose>=0 && choose<=sizeOfmenuArray)
  {
    carrayPtr=map(carrayPtr,5,menu[choose].fun);
    
    printf("Please choose a function:\n");
    for (size_t i = 0; i < sizeOfmenuArray; i++)
    {
      printf("%d) %s\n",i,menu[i].name);
    }
    char buff2[10];
    fgets(buff2,5,stdin);
    choose=atoi(buff2);
  }
  free(carrayPtr);
  
  exit(0);
}