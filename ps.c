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
    if(argc>1){
        printf(1,"please dont give this guy arguments, he can't debate\n");
    }
    else{
        ps();
    }
    exit();
}