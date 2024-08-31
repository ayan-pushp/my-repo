/*
============================================================================
Name : 18_1.c
Author : Ayan Pushp
Description : Creating database of 3 train records.
Date: 31st Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

 int main(){
  int fd,i;
  struct{
    int tr_no;
    int t_count;
    }db[3];
  
  for(i=0;i<3;i++){
    db[i].tr_no = i+1;
    db[i].t_count = 0;
    }
    
  fd = open("Train_record.txt",O_CREAT | O_RDWR, 0744);
  write(fd, db, sizeof(db));
  
  return 0;
  }
