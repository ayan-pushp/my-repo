/*
============================================================================
Name : 17_1.c
Author : Ayan Pushp
Description : Write a program to simulate online ticket reservation. This program opens a file, stores a ticket number and exits.
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
  int fd,i;
  db.t_no = 5;
    
  fd = open("Train_record.txt",O_CREAT | O_RDWR, 0744);
  write(fd, db, sizeof(db));
  close(fd);
  
  fd = open("Train_record.txt",O_RDONLY);
  read(fd,&db,sizeof(db));
  printf("Ticket no: %d\n",db.t_no);
  close(fd);
  return 0;
  }
