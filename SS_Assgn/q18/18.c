/*
============================================================================
Name : 18.c
Author : Ayan Pushp
Description : Write a program to perform Record locking.
a. Implement write lock
b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access
to avoid race condition.
Date: 31st Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

struct{
    int tr_no;
    int t_count;
    }db;
  
 int main(){
  int fd,ip,i;
  fd = open("Train_record.txt",O_RDWR, 0744);
  
  printf("Select train number: 1,2,3\n");
  scanf("%d",&ip);
  
  struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = (ip-1)*sizeof(db);
  lock.l_len = sizeof(db);
  lock.l_pid = getpid();
  
  lseek(fd, (ip-1)*sizeof(db),SEEK_SET);
  read(fd,&db,sizeof(db));
  printf("Critical section...\n");
  
  fcntl(fd,F_SETLKW,&lock);
  printf("Current ticket count:%d\n",db.t_count);
  db.t_count++;
  
  lseek(fd,-1*sizeof(db),SEEK_CUR);
  write(fd,&db,sizeof(db));
  printf("To book ticket,press enter");
  getchar();
  
  getchar();
  lock.l_type =F_UNLCK;
  fcntl(fd,F_SETLK,&lock);
  
  printf("%d tickets booked in train %d\n",db.t_count,db.tr_no);
  }
