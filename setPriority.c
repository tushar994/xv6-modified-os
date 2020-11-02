#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

int main(int argc, char* argv[]){
    if(argc!=3){
        printf(1,"invalid numebr of arguments\n");
        exit();
    }

    int len1=0;
    while(argv[1][len1]!='\0'){
        len1++;
    }
    int int1 = 0;
    for(int i=0;i<len1;i++){
        int1*=10;
        int1 += (int)argv[1][i] - (int)'0';
    }
    int len2=0;
    while(argv[2][len2]!='\0'){
        len2++;
    }
    int int2 = 0;
    for(int i=0;i<len2;i++){
        int2*=10;
        int2 += (int)argv[2][i] - (int)'0';
    }
    if(int1<0 || int1>100){
        printf(1,"please give valid priority\n");
        exit();
    }

    set_priority(int1,int2);
    

    exit();

}