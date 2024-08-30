#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    if(!fork()){
    printf("In child -PID =%d\n",getpid());
    }
    else{
    printf("This is parent process -PID =%d\n",getpid());
    sleep(30);
    wait(0);
    }
    
    return 0;
}

