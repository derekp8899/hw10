/*
Derek Popowski
derek.a.popowski@und.edu
CSci 451 Operating Systems
HW10
12-3-18

program1.c
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

  char *filename = argv[1];
  char *pipe1write = argv[2];
  char *semId = argv[3];
  char *wordcount = malloc(50);//input args
  sprintf(wordcount,"wc -c %s",filename);//command the get the input file size
  FILE *wc = popen(wordcount, "r");//pipe to use wc for char count of the input file
  int charCount;//where to store the file size
  fscanf(wc,"%d",&charCount);//grab the int value for the char count
  pclose(wc);//close the pipe
  //printf("size of the input file is : %d\n",charCount);
  sem_t *sem1 = sem_open(semId,O_CREAT);//open semaphore 1
  int file = open(filename,O_RDONLY);//open the input file
  char *buffer = malloc(charCount);//input file data buffer
  read(file,buffer,charCount);//read the data in from the file
  char *word = malloc(250);//word buffer
  int p1W = atoi(pipe1write);//get the pipe1read FD
  word = strtok(buffer," ");//grab first word from the input buffer
  char *word2 = malloc(250);//word buffer for formatting
  while (word != NULL){//while there are still words
    int semval;
    sem_getvalue(sem1,&semval);//grab the semaphore value
    while(semval == 1){//wait until program 2 has read the last word
      sleep(.1);
      sem_getvalue(sem1,&semval);
    }
    sprintf(word2,"%s\n%c",word,'\0');//one word per line
    write(p1W,word2,100);//write to pipe1
    sem_post(sem1);//increment the semaphore value
    word = strtok(NULL," ");//get the next word from the file input buffer
  }
  sem_post(sem1);//increment the value of semaphore for final time
  close(p1W);//close the write end of pipe 1
  sem_close(sem1);//close the semaphore
  free(buffer);free(word);free(word2);//free the string buffers
  exit(0);
}
