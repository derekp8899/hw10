/*
Derek Popowski
derek.a.popowski@und.edu
CSci 451 Operating Systems
HW10
12-3-18

program3.c
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
#include <semaphore.h>

int main(int argc, char* argv[]){

  char *pipe2read = argv[1];
  char *sem2Id = argv[2];
  char *shmId = argv[3];
  char *outName = argv[4];
  int p2R = atoi(pipe2read);
  char *buffer= malloc(100);
  printf("program3\n");
  sem_t *sem2 = sem_open(sem2Id,O_CREAT);
  sem_wait(sem2);
  while(read(p2R,buffer,100)>0){
    printf("%s");
    sleep(.5);
  }
  int *spoint;
  spoint = shmat(atoi(shmId),NULL,0);
  printf("number of writes in program 2 %d \n",*spoint);
  shmdt(spoint);
  close(p2R);
  exit(0);
}
