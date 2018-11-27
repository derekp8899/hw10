/*
Derek Popowski
derek.a.popowski@und.edu
CSci 451 Operating Systems
HW10
12-3-18

program2.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char* argv[]){

  char *pipe1read = argv[1];
  char *pipe2write = argv[2];
  char *sem1Id = argv[3];
  char *sem2Id = argv[4];
  char *shmKey = argv[5];

}
