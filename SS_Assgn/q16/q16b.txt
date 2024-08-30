#include <stdio.h>
#include <fcntl.h>

int main(){
struct flock lock;
int fd=open("/home/ayan/demo/my-repo/file1.txt", O_RDWR);

lock.l_type = F_RDLCK;
lock.l_whence = SEEK_SET;
lock.l_start = 0;
lock.l_len = 0;
lock.l_pid = getpid();

printf("Before entering critical section\n");
fcntl(fd,F_SETLKW,&lock);
printf("Inside critical section\n");
printf("Press Enter to unlock");
getchar();
printf("File unlocked\n");
lock.l_type = F_UNLCK;
fcntl(fd, F_SETLK, &lock);
printf("End\n");

return 0;
}
