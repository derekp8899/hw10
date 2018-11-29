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
  char *wordcount = malloc(50);
  sprintf(wordcount,"wc -c %s",filename);
  FILE *wc = popen(wordcount, "r");//pipe to use wc for char count of the input file
  int charCount;
  fscanf(wc,"%d",&charCount);//grab the int value for the char count
  pclose(wc);//close the pipe
  printf("size of the input file is : %d\n",charCount);
  sem_t *sem1 = sem_open(semId,O_CREAT);
  int file = open(filename,O_RDONLY);//open the input file
  char *buffer = malloc(charCount);//input file data buffer
  read(file,buffer,charCount);//read the data in from the file

  char *word = malloc(250);//word buffer
  //  system("touch temp1.data");//create the temp1.data file
  //int temp1 = open("temp1.data",O_WRONLY);//open temp1 for writing
  int p1W = atoi(pipe1write);//get the pipe1read FD
  word = strtok(buffer," ");//grab first word from the input buffer
  printf("program1\n\n");
  char *word2 = malloc(250);//word buffer for formatting
  //sem_post(sem1);
  while (word != NULL){//while there are still words
    int semval;
    sem_getvalue(sem1,&semval);
    if(semval == 1)
      sleep(.2);
    sprintf(word2,"%s\n\0",word);//one word per line
    //sem_wait(sem1);
    write(p1W,word2,100);//write to pipe1
    sem_post(sem1);
    sleep(1);
    printf("%s",word2);//verify the word
    word = strtok(NULL," ");
  }
  close(p1W);
  sem_close(sem1);
  free(buffer);free(word);free(word2);//free the string buffers
  //  printf("temp1.data written\n");
  exit(0);
}
