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
    int wtime,rtime;

    int pid = fork();
    if(pid==0){
        exec(argv[1],argv + 1);
        exit();
    }

    waitx(&wtime,&rtime);

    printf(1, "\nIt waited for - %d\n",wtime);
    printf(1, "\nIt ran for - %d\n",rtime);

    exit();

    return 0;
}