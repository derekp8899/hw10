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
  char *outName = argv[4];//input args
  int p2R = atoi(pipe2read);//get the pipeid
  char* command = malloc(500);//system command buffer
  sprintf(command,"touch %s",outName);
  system(command);//create the output file
  free(command);//free the command buffer
  char *buffer= malloc(100);//pipe read buffer
  char *word = malloc(100);//buffer for reading and writing
  //  printf("program3\n");//debug print
  int outFile = open(outName,O_WRONLY);//open the outfile for writing
  sem_t *sem2 = sem_open(sem2Id,O_CREAT);//open the semaphore
  sem_wait(sem2);//wait until program2 writes to the pipe
  while(read(p2R,buffer,100)>0){//read until all the write ends are closed
    //printf("%s",buffer);//print what was read from the pipe
    word = strtok(buffer,"\n");
    sprintf(word,"%s ",word);//format the word for printing to the file
    write(outFile,word,strlen(word));//write the to output file
    int semval;
    sem_getvalue(sem2,&semval);//grab the semaphore value
    while(semval==0){//if program2 has not written to the file or exit
      sleep(.1);
      sem_getvalue(sem2,&semval);
    }
    sem_wait(sem2);//decrement the semaphore
  }
  write(outFile,"\n",1);//write a final newline to the output file
  int *spoint;//pointer to mount the shared memory
  spoint = shmat(atoi(shmId),NULL,0);//open pointer to the shared mem
  printf("Type 1 : %d \n",*spoint);
  printf("Type 2 : %d \n",*(spoint+1));//print the word 'types'
  shmdt(spoint);//detatch the shared mem
  close(p2R);//close the pipe end
  close(outFile);//close the output file
  free(buffer);//free(word);//free the char buffers
  exit(0);
}
