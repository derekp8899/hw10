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
#include <semaphore.h>
char *formatWord(char* buffer,int* spoint);//to format words read in from the pipe
int main(int argc, char* argv[]){

  char *pipe1read = argv[1];
  char *pipe2write = argv[2];
  char *sem1Id = argv[3];
  char *sem2Id = argv[4];
  char *shmId = argv[5];//input args
  int p1R = atoi(pipe1read);
  int p2W = atoi(pipe2write);//the two used pipe ends
  sem_t *sem1 = sem_open(sem1Id,O_CREAT);
  sem_t *sem2 = sem_open(sem2Id,O_CREAT);//open the two semaphores
  char *buffer = malloc(100);//input buffer
  int *spoint;//pointer to the shared memory
  spoint = shmat(atoi(shmId),NULL,0);//attach the shm
  *spoint=0;
  *(spoint+1)=0;//init the shared memory counters
  sem_wait(sem1);//wait for first write to pipe 1
  char *word = malloc(250);//word temp storage buffer
  char *word2 = malloc(250);//word buffer for formatting
  while(read(p1R,buffer,100) > 0){//while the pipe is still being written to
    word = formatWord(buffer,spoint);//format the word
    int semval2;//semaphore 2 value
    sem_getvalue(sem2,&semval2);//get the sem2 value
    while(semval2==1){//while program3 has not read in the word
      sleep(.1);
      sem_getvalue(sem2,&semval2);
    }
    write(p2W,word,100);//write to pipe2
    sem_post(sem2);//increment semaphore 2
    int semval1;//semaphore 1 value
    sem_getvalue(sem1,&semval1);//get the sem1 value
    while(semval1==0){//while program 1 has not written a new word to pipe1 or exit
      sleep(.1);
      sem_getvalue(sem1,&semval1);
    }
    sem_wait(sem1);//decrement the value of sem 1
  }
  sem_post(sem2);//final post to semaphore 2 

  close(p1R);close(p2W);sem_close(sem1);sem_close(sem2);//close the pipe ends and semaphores
  shmdt(spoint);//detach the shm
  free(buffer);free(word);free(word2);//free(sharedbuf);//free the string buffers
  return(0);

}
char *formatWord(char *buffer, int *spoint){//format the words for 'pig latin'

  char* word = malloc(100);
  word = strtok(buffer,"\n");//grab word from the input buffer
  char* word2 = malloc(100);
  if(word[0] == 65||word[0] == 69||word[0] == 73||word[0] == 79||word[0] == 85||word[0] == 97||word[0] == 101||word[0] == 105||word[0] == 111||word[0] == 117){//if word starts with a vowel (upper or lower)
    char temppunc;
    temppunc = word[strlen(word)-1];
    if(temppunc <65){
      word[strlen(word)-1] = '\0';
      sprintf(word2,"%sray%c\n",word,temppunc);//format the word

    }
    else
      sprintf(word2,"%sray\n",word);//format the word
    (*spoint)++;//inc 'type1'
  }
  else{
    
    int i = 0;
    
    char temp,temppunc;
    temp = word[0];
    temppunc = word[strlen(word)-1];//temp store first and last characters for formatting
    
    for (i = 1; i < (strlen(word)); i++){//loop word and shift
      
      if(word[i] < 65)//if we find a puncuation symbol
	break;
      word[i-1] = word[i];//shift the word
      
    }
    
    if(temppunc < 65){// if there was punctuation at the end of the word
      word[strlen(word)-2] = temp;//move frist character to the end
      word[strlen(word)-1] = '\0';//null term
      sprintf(word2,"%say%c\n",word,temppunc);//format the word adding back the punctuation
    }
    else{//no punctuation
      word[strlen(word)-1] = temp;//move frist character to the end
      sprintf(word2,"%say\n",word);//format the word
    }
    *(spoint+1)+=1;//inc 'type2'
  }
  //  printf("%s",word2);//print the word to verify
  return word2;
}
