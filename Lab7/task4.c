#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>


int digit_counter (char* ch) {
    int counter =0;
    

    for(int i=0; i<strlen(ch); i++){
        if(ch[i]>= '0' && ch[i]<= '9'){
            counter++;
        }
    }

    return counter;

}


int main(int argc, char **argv){
    
    if(argc>1){
        int ans = digit_counter(argv[1]);
        printf("%d\n", ans);
    }

    exit(0);
}