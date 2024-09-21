/*
============================================================================
Name : 4.c
Author : Ayan Pushp
Description : Write a program to measure how much time is taken to execute 100 getppid ( ) system call. Use time stamp counter.
Date: 20th Sep, 2024.
============================================================================
*/
#include<stdio.h>

unsigned long long rdtsc()
{
  unsigned long long dst;
  __asm__ __volatile__("rdtsc":"=A"(dst));
  return dst;
}
int main()
{
  long long int start,end;
  // Start timing
  start=rdtsc();
  
  // Call getppid() 100 times
  for (int i = 0; i < 100; i++)  getppid();
  
  // End timing
    end = rdtsc();
    
    printf("Time taken to execute 100 getppid() calls: %llu cycles %f seconds\n",end-start,(end-start)/1600000000.0);
    
    return 0;
  }
