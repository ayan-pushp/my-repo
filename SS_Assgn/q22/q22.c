#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>

#define FILE "/home/ayan/my_file.txt"
#define MESSAGE_PARENT "Parent Process message.\n"
#define MESSAGE_CHILD "Child Process message.\n"

int main(){
int fd=open(FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
if(fd<0){
perror("open");
return 1;
}
pid_t pid=fork();

if(pid<0){
perror("fork");
close(fd);
return 1;
}
else if(pid==0){
if(write(fd,MESSAGE_CHILD,strlen(MESSAGE_CHILD))<0){
perror("write");
}
close(fd);
printf("Child wrote");
}
else{
if(write(fd,MESSAGE_PARENT,strlen(MESSAGE_PARENT))<0){
perror("write");
}
close(fd);
printf("Parent wrote");
wait(NULL);
}
return 0;
}

