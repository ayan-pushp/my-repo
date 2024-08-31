/*
============================================================================
Name : 17_2.c
Author : Ayan Pushp
Description : Write a program to simulate online ticket reservation. This program program opens the created file, implements write lock, reads the ticket number, increments the number, prints the new ticket number and then closes the file.
Date: 31st Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

struct{
    int t_no;
    }db;
    
 int main(){
  struct flock lock;
  int fd;
    
  fd = open("Train_record.txt",O_RDWR);
  lock.l_type=F_WRLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=0;
  lock.l_len=sizeof(db);
  lock.l_pid=getpid();
  printf("Before Critical Section...\n");
  fcntl(fd, F_SETLKW, &lock);
  
  printf("Inside Critical Section.\n");
  read(fd,&db,sizeof(db));
  printf("Current ticket no: %d\n",db.t_no);
  db.t_no++;
  
  lseek(fd,0,SEEK_SET);
  write(fd,&db, sizeof(db));
  
  printf("Press Enter to unlock\n");
  getchar();
  
  lock.l_type = F_UNLCK;
  fcntl(fd,F_SETLKW,&lock);
  printf("Exited Critical Section.\n");
  close(fd);
  
  return 0;
  }
