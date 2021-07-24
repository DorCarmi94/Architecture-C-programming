#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


int debugMode=0;
int displayMode=0;
int CurrentFd=0;
char *addr;

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



void PrintRelocationTable (state *st)
{
    Elf32_Ehdr *ehdr = (Elf32_Ehdr*)addr;
  Elf32_Shdr *shdr = (Elf32_Shdr *)(addr + ehdr->e_shoff);
  int shnum = ehdr->e_shnum;

  Elf32_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
  const char *const sh_strtab_p = addr + sh_strtab->sh_offset;
    int idodoindex=0;
    Elf32_Shdr *cur_sh;
    Elf32_Shdr *cur_shReal;//rel.dyn
    Elf32_Shdr *strrrr_sh;
    Elf32_Shdr *dysm;


    for (size_t i = 0; i < shnum; i++)
    {
        cur_sh=&shdr[i];

        if(cur_sh->sh_type ==SHT_STRTAB)
         {
            strrrr_sh = &shdr[i];
            
         }
         if(cur_sh->sh_type ==SHT_DYNSYM)
         {
            dysm = &shdr[i];
         }


    }

    for (size_t i = 0; i < shnum; i++)
    {
        cur_sh=&shdr[i];
         if(cur_sh->sh_type==SHT_REL)
         {         
             cur_shReal=cur_sh;
             idodoindex=i;
         
        Elf32_Shdr *sh_relTab=&shdr[idodoindex];
        int numOfIdodods=cur_shReal->sh_size/sizeof(Elf32_Rel);
        Elf32_Rel *relPtr=(Elf32_Rel*)(addr+sh_relTab->sh_offset);

        for(int j=0; j<numOfIdodods; j++){
            printf("[%2d] ", j);
            printf("%X\t",relPtr->r_offset);
            printf("%X\t",relPtr->r_info);
            int info=relPtr->r_info;
            printf("%x\t",ELF32_R_TYPE(info));
            int symOff=ELF32_R_SYM(info);
            // int dysmOff=addr+dysm->sh_offset;
            Elf32_Sym *symPtr=(Elf32_Sym*)(addr + dysm->sh_offset);
            symPtr=symPtr+symOff;
            int value=symPtr->st_value;

            //printf("%x\t",addr + strrrr_sh->sh_offset + value );

            printf("the offset issss %x\t", strrrr_sh->sh_offset );


            printf("%x\t", value );
            
            // int name = symPtr->st_name;

            printf("%s\t",addr + strrrr_sh->sh_offset + symPtr->st_name );


            // printf("%x\t",ELF32_R_SYM(info));

            printf("\n");

            relPtr++;
        }

        printf("Number of entries rels:%d\n",numOfIdodods);
         }
    }
}


void PrintSymbolTable (state *st)
{
   Elf32_Ehdr *ehdr = (Elf32_Ehdr*)addr;
  Elf32_Shdr *shdr = (Elf32_Shdr *)(addr + ehdr->e_shoff);
  int shnum = ehdr->e_shnum;

  Elf32_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
  const char *const sh_strtab_p = addr + sh_strtab->sh_offset;
    int idodoindex=0;
    Elf32_Shdr *cur_sh;
    Elf32_Shdr *cur_shReal;
    Elf32_Shdr *strrrr_sh;


    for (size_t i = 0; i < shnum; i++)
    {
        cur_sh=&shdr[i];
         if(strcmp(sh_strtab_p+cur_sh->sh_name,".strtab")==0)
         {
             strrrr_sh = &shdr[i];
            
         }


    }
    
    for (size_t i = 0; i < shnum; i++)
    {
        cur_sh=&shdr[i];
        
         if(cur_sh->sh_type==SHT_SYMTAB)
         {
             myPrintFunc(i,cur_sh,cur_sh,strrrr_sh,shdr,shnum,sh_strtab_p);
         }
    }

    
}

void myPrintFunc(int idodoindex,Elf32_Shdr * cur_sh,Elf32_Shdr * cur_shReal,
Elf32_Shdr *strrrr_sh,Elf32_Shdr *shdr, int shnum, const char *const sh_strtab_p)
{
    
    Elf32_Shdr *sh_symTab=&shdr[idodoindex];
    int numOfIdodods=cur_shReal->sh_size/sizeof(Elf32_Sym);
    Elf32_Sym *symPtr=(Elf32_Sym*)(addr+sh_symTab->sh_offset);




    for(int j=0; j<numOfIdodods; j++){
        printf("[%2d] ", j);
        printf("%x\t",symPtr->st_value);
        printf("%d\t",symPtr->st_shndx);
        if(symPtr->st_shndx >=0 && symPtr->st_shndx <=shnum){
            printf("%s\t",sh_strtab_p + shdr[symPtr->st_shndx].sh_name);
        }
        else
        {
            printf("\t");
        }
        printf("%s\n",addr + strrrr_sh->sh_offset + symPtr->st_name);
        symPtr++;
    }
    printf("Number of symbols:%d\n",numOfIdodods);
}

void PrintSectionNames (state *st)
{

  Elf32_Ehdr *ehdr = (Elf32_Ehdr*)addr;
  Elf32_Shdr *shdr = (Elf32_Shdr *)(addr + ehdr->e_shoff);
  int shnum = ehdr->e_shnum;

  Elf32_Shdr *sh_strtab = &shdr[ehdr->e_shstrndx];
  const char *const sh_strtab_p = addr + sh_strtab->sh_offset;

  for (int i = 0; i < shnum; ++i) {
    printf("[%2d] %s %x %08x %4d %d\n", i,sh_strtab_p + shdr[i].sh_name, 
                    (unsigned int) (shdr[i].sh_addr), 
                    shdr[i].sh_offset,
                    shdr[i].sh_size,
                    shdr[i].sh_type
           );
  }
}


void ExamineELF (state *st)
{
    char fileName[100];
    fgets(fileName,100,stdin);
    //const char file_n_ptr=fileName;
    char entryPoint[4];
    char newFileName[100];
    for (size_t i = 0; i < strlen(fileName); i++)
    {
        if(fileName[i]!='\n')
        {
            newFileName[i]=fileName[i];
        }
        else
        {
            newFileName[i]='\0';
            break;
        }
        
    }
    
    printf("filename: %s",newFileName);
           
           struct stat sb;
           off_t offset, pa_offset;
           size_t length;
           ssize_t s;

           CurrentFd = open(newFileName, O_RDONLY);
           if (CurrentFd == -1)
               handle_error("open");

           if (fstat(CurrentFd, &sb) == -1)           /* To obtain file size */
               handle_error("fstat");

           offset = 0;
           
           pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
               /* offset for mmap() must be page aligned */

             /* No length arg ==> display to end of file */
            length = sb.st_size - offset;
           
            
           addr = mmap(NULL, length + offset - pa_offset, PROT_READ,
                       MAP_PRIVATE, CurrentFd, pa_offset);
           if (addr == MAP_FAILED)
           {
               munmap(addr, length + offset - pa_offset);
                close(CurrentFd);
               handle_error("mmap");
               
           }
       Elf32_Ehdr *ehdr = (Elf32_Ehdr*)addr;
        //    s = write(STDOUT_FILENO, addr + offset - pa_offset, length);
        //     printf("\n\n****\n\n");
        //    if (s != length) {
        //        if (s == -1)
        //            handle_error("write");
                
        //        fprintf(stderr, "partial write");
        //        exit(EXIT_FAILURE);
        //    }
        printf("\n\n\nMagic: %x,%x,%x\n",addr[0],addr[1],addr[2]);
        
        printf("Entrypoint: %x\n",ehdr->e_entry);

        printf("Data encoding:%x\n",addr[5]);

        printf("The file offset in which the section header table resides: %x\n",ehdr->e_shoff);

        printf("The number of section header entries: %d\n",ehdr->e_shnum);
        printf("The size of each section header entry: %d\n",ehdr->e_shentsize);

        printf("The file offset in which the program header table resides: %x\n",ehdr->e_phoff );
        printf("The number of program header entries: %x%x\n",ehdr->e_phnum);

        
        printf("The size of each program header entry: %d\n",ehdr->e_phentsize);

        
        //check about each section and each program header entry size
}

void map(state *s, int array_length, char (*f) (state*));

void map(state *s, int array_length, char (*f) (state*)){
        f(s);
}


int main(int argc, char **argv){
  state *idoState=calloc(1,sizeof(state));


  struct fun_desc menu[] = {    { "Toggle Debug Mode", ToggleDebugMode },
                                { "Examine ELF file", ExamineELF },
                                { "Print Section Names", PrintSectionNames },
                                {"Print Symbol Table",PrintSymbolTable},
                                { "Print Relocation Table", PrintRelocationTable },
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