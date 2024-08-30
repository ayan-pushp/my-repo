#include <stdio.h>
#include <stdlib.h>

int main() {

    if(!fork()){
      printf("In child -Before orphan PPID = %d\n",getppid());
      sleep(1);
      printf("In child -Afterorphan PPID =%d\n",getppid());
      }
      else{
      printf("This is parent process %d\n",getpid());
        exit(0);
    }

    return 0;
}

